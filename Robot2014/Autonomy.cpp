#include "Autonomy.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <unistd.h>

static int g_whatsHot = 0;
static double s_lastLeftDist = 0.0;
static double s_lastRightDist = 0.0;

//////////////////////////////////////////// IdleState ////////////////////////////////////////////
IdleState::IdleState()
{
  m_msSpentIdle = 0;
  // NOTE: See here that if this default c'tor is used without changing m_msToSpendIdle,
  // the Idle state will run indefinitely
  m_msToSpendIdle = INT_MAX;
}

IdleState::IdleState(EntropyDrive& _entDrive, double _timeToSpendIdle)
{
  m_msSpentIdle = 0;
  m_msToSpendIdle = _timeToSpendIdle;
  
  m_entDrive = &_entDrive;
}

void IdleState::setTimeToSpendIdle(double _timeToSpendIdle)
{
  m_msToSpendIdle = _timeToSpendIdle;
}

bool IdleState::Update(double _dt)
{
  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Idle %f | %f", m_msSpentIdle, _dt);
  DriverStationLCD::GetInstance()->UpdateLCD();
	  
  m_entDrive->DriveRobot(0, 0);
  
  // Sum the current time spent idle with the time between this frame and the last frame (delta),
  // and return whether it's exceeded the specified time to spend idle
  return _dt > m_msToSpendIdle;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////// DecisionState ////////////////////////////////////////////
DecisionState::DecisionState()
{
}

DecisionState::DecisionState(EntropyDrive& _entDrive)
{ 
  m_entDrive = &_entDrive;
  leftOrRight = 0;
  	camera = &AxisCamera::GetInstance();
}

bool DecisionState::Update(double _dt)
{
  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Decision");
  DriverStationLCD::GetInstance()->UpdateLCD();
	  
  m_entDrive->DriveRobot(0, 0);
  g_whatsHot = Camera();
  // Sum the current time spent idle with the time between this frame and the last frame (delta),
  // and return whether it's exceeded the specified time to spend idle
  return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////// PositionState //////////////////////////////////////////

PositionState::PositionState()
{
  m_entDrive = 0;
  
  m_currentPos.x = 0.0;
  m_currentPos.y = 0.0;
  m_forwardSpeed = 0.0;
  m_rotation = 0.0;
  m_rotCorrection = 0.0;
  m_scalingFactor = 0.0;
  m_deltaLeftDist = 0.0;
  m_deltaRightDist = 0.0;
}

PositionState::PositionState(EntropyDrive& _entDrive, Vec2 _targetPos, Encoder* _leftEncoder, Encoder* _rightEncoder,  double _targetRotation)
{
  m_entDrive = &_entDrive;
	
  m_targetPos = _targetPos;

  m_leftEncoder = _leftEncoder;
  m_rightEncoder = _rightEncoder;
  
  m_currentPos.x = 0.0;
  m_currentPos.y = 0.0;
  m_targetRotation = _targetRotation;
  m_forwardSpeed = -0.30;
  m_rotation = 0.0;
  m_rotCorrection = 0.0;
  m_scalingFactor = 0.2;
  m_deltaLeftDist = 0.0;
  m_deltaRightDist = 0.0;
}

void PositionState::Init()
{
  m_currentPos.x = 0.0;
  m_currentPos.y = 0.0;
  m_rotation = 0.0;
  m_rotCorrection = 0.0;
  m_deltaLeftDist = 0.0;
  m_deltaRightDist = 0.0;
}

bool PositionState::Update(double _dt)
{
  double leftEncDist = m_leftEncoder->GetDistance();
  double rightEncDist = m_rightEncoder->GetDistance();
  leftEncDist *= -1.0;

  m_deltaLeftDist = leftEncDist - s_lastLeftDist;
  m_deltaRightDist = rightEncDist - s_lastRightDist;

  m_currentPos.x += m_deltaLeftDist;
  m_currentPos.y += m_deltaRightDist;
  
  m_rotCorrection = (m_deltaLeftDist - m_deltaRightDist + m_rotCorrection);// * m_scalingFactor;

  if(((m_currentPos.x + m_currentPos.y) / 2) * -1 > m_targetPos.x)
  {
	  m_entDrive->DriveRobot(1.0, 0);
	  return true;
  }
  else
  {
	  m_entDrive->DriveRobot(m_forwardSpeed, m_rotCorrection);
	
	  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Positioning");
	  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line4, "Rotation Auto: %f", m_rotCorrection);
	  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line5, "Left Dist: %f", m_leftEncoder->GetDistance());
	  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line6, "Right Dist: %f", m_rightEncoder->GetDistance());
	  DriverStationLCD::GetInstance()->UpdateLCD();
	  
	  s_lastLeftDist = leftEncDist;
	  s_lastRightDist = rightEncDist;
	
	  return false;
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////// RotationState //////////////////////////////////////////

RotationState::RotationState()
{
  m_entDrive = 0;

  m_rotation = 0.0;
  m_deltaLeftDist = 0.0;
  m_deltaRightDist = 0.0;
}

RotationState::RotationState(EntropyDrive& _entDrive, Encoder* _leftEncoder, Encoder* _rightEncoder,  double _targetRotation, Gyro *_Gyro)
{
  m_entDrive = &_entDrive;	 
  m_Gyro = _Gyro;
  m_leftEncoder = _leftEncoder;
  m_rightEncoder = _rightEncoder;
  
  m_targetRotation = _targetRotation;
  m_rotation = 0.0;
  m_deltaLeftDist = 0.0;
  m_deltaRightDist = 0.0;
  
  
}

void RotationState::Init()
{
  m_rotation = 0.0;
  m_deltaLeftDist = 0.0;
  m_deltaRightDist = 0.0;
}

bool RotationState::Update(double _dt)
{	
	static float rotationEpoch = m_Gyro->GetAngle();
	
	if (g_whatsHot == 1)
		m_entDrive->DriveRobot(0, -0.3);
	
	else if (g_whatsHot == -1 || g_whatsHot == 0)
		m_entDrive->DriveRobot(0, 0.3);	
	 DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "Voltage : %f",m_Gyro->GetRate());
     DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line2, "Angle : %f", m_Gyro->GetAngle());
	 DriverStationLCD::GetInstance()->UpdateLCD();
	
	if (g_whatsHot == -1)
	{
		return (m_Gyro->GetAngle() - rotationEpoch >=  m_targetRotation);//(m_rotation += m_Gyro->GetAngle()) <= m_targetRotation;
	}
	if (g_whatsHot == 1)
		{
			return (m_Gyro->GetAngle() - rotationEpoch <=  -m_targetRotation);//(m_rotation += m_Gyro->GetAngle()) <= m_targetRotation;
		}
}
///////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////// ShootState ///////////////////////////////////////////
bool ShootState::Update(double _dt){ return true; }

///////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////// Autonomy /////////////////////////////////////////////
Autonomy::Autonomy()
{
}

Autonomy::Autonomy(EntropyDrive& _entDrive, Gyro *_Gyro)
{
	m_analogChan = new AnalogChannel (IODefinitions::AUTONOMOUS_SCENARIO_STATE);
	
	m_Gyro = _Gyro;
	
	// TODO: Might not want it in feet
	m_leftEncoder = new Encoder(1, 1, 1, 2, false, Encoder::k4X);
	m_rightEncoder = new Encoder(1, 3, 1, 4, true, Encoder::k4X);
	
	m_leftEncoder->SetDistancePerPulse(PULSE_RATIO / 12.0);
	m_leftEncoder->SetPIDSourceParameter(Encoder::kRate);
	m_rightEncoder->SetDistancePerPulse(PULSE_RATIO / 12.0);
	m_rightEncoder->SetPIDSourceParameter(Encoder::kRate);
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line6, "2");
			 DriverStationLCD::GetInstance()->UpdateLCD();
					 
	m_leftEncoder->Start();
	m_rightEncoder->Start();

	
	if(m_analogChan->GetVoltage() < 0.35)
	{
		  m_statesToComplete.push_back(new IdleState(_entDrive, 50000000));	
		  m_statesToComplete.push_back(new RotationState(_entDrive, m_leftEncoder, m_rightEncoder, 14, m_Gyro));	
		  m_statesToComplete.push_back(new DecisionState(_entDrive));
		  m_statesToComplete.push_back(new PositionState(_entDrive, Vec2(9.5, 0), m_leftEncoder, m_rightEncoder));
	}
	else
	{
		m_statesToComplete.push_back(new IdleState(_entDrive, 50000000));
		  m_statesToComplete.push_back(new PositionState(_entDrive, Vec2(9.0, 0), m_leftEncoder, m_rightEncoder));
	}

  m_currentState = m_statesToComplete.back();
  m_currentState->Init();
  
}

void Autonomy::StartNextState()
{
  if(m_statesToComplete.size())
  {
	//delete m_statesToComplete.back();
    m_statesToComplete.pop_back();
    m_currentState = m_statesToComplete.back();
  }
}
  
void Autonomy::Update(double _dt)
{
  if(m_currentState->Update(_dt))
  {
    StartNextState();
    m_currentState->Init();
  }
}


uint32_t DecisionState::readFileSize(std::ifstream& _ifl)
{
	// read the size of the file by seeking to the end and getting the pointer count
	_ifl.seekg(0, _ifl.end);
	uint32_t size = _ifl.tellg();
	// return the pointer to the beginning of the file
	_ifl.seekg(0, _ifl.beg);

	return size;
}

uint32_t DecisionState::readDimension(std::ifstream& _ifl, uint32_t _offset)
{
	// read the dimensions of the image
	int8_t dimRead[4] = { 0 };
	_ifl.seekg(_offset);
	_ifl.read((char*)dimRead, 4);
	// get us back to the beginning of the file
	_ifl.seekg(0, _ifl.beg);

	// the dimensions are contained within four 8-bit values
	// so bit shift them, appropriately
	uint32_t dim = (uint8_t)dimRead[0];
	dim += (uint8_t)dimRead[1] << 8;
	dim += (uint8_t)dimRead[2] << 16;
	dim += (uint8_t)dimRead[3] << 24;

	return dim;
}

void DecisionState::convertToGrayscale(rgbTrip**& _pixArray, uint32_t _xSize, uint32_t _ySize)
{
	for (uint32_t y = 0; y < _ySize; ++y)
	{
		for (uint32_t x = 0; x < _xSize; ++x)
		{
			uint8_t lum = (_pixArray[x][y].red * RED_LUM) + (_pixArray[x][y].green * GREEN_LUM) + (_pixArray[x][y].blue * BLUE_LUM);

			_pixArray[x][y].red = lum;
			_pixArray[x][y].green = lum;
			_pixArray[x][y].blue = lum;
		}
	}
}
int DecisionState::Camera()
{
	std::ifstream ifl("potato3.bmp", std::ios::binary);
	if(ifl.is_open())
	{
		// we'll want to know how much memory we should reserve to read in the image
		uint32_t sizeOfFile = readFileSize(ifl);

	    DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "file size: %d", sizeOfFile);
	    DriverStationLCD::GetInstance()->UpdateLCD();
	    
		// we'll want to know the dimensions of the image
		uint32_t xDim = readDimension(ifl, 18);
		uint32_t yDim = readDimension(ifl, 22);

		// ask to borrow some memory
		uint8_t* image = new uint8_t[sizeOfFile];
		//uint8_t image[3126];// = new uint8_t[sizeOfFile];

		// read in the entirety of the image
		ifl.read((char*)image, sizeOfFile);
		// let go of the file
		ifl.close();
	
		std::ifstream iflgray("potato3.bmp", std::ios::binary);
	
		// we'll want to know how much memory we should reserve to read in the image
		uint32_t sizeOfFilegray = readFileSize(iflgray);

		// we'll want to know the dimensions of the image
		uint32_t xDimgray = readDimension(iflgray, 18);
		uint32_t yDimgray = readDimension(iflgray, 22);

		// ask to borrow some memory
		uint8_t* imagegray = new uint8_t[sizeOfFilegray];
		//uint8_t image[3126];// = new uint8_t[sizeOfFile];

		// read in the entirety of the image
		iflgray.read((char*)imagegray, sizeOfFilegray);
		// let go of the file
		iflgray.close();

	    DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "pic saved (i hop");
	    DriverStationLCD::GetInstance()->UpdateLCD();
		// again, ask to borrow some memory for this
		// because we want a two-dimensional array, we have to make this a double pointer
		// so first allocate memory the x-axis (i.e., column). then, loop through each column and allocate
		// a number of rows (i.e., y-axis) for that column 
		rgbTrip** pixels = new rgbTrip*[xDim];
		for (uint32_t x = 0; x < xDim; ++x)
		{
			pixels[x] = new rgbTrip[yDim];
		}


		// convert the unsigned char one-dimensional array into a 
		// two-dimensional array of RGBs so it's easier to work with
		// note: we're reading left to right here, so the x-axis is the inner loop
		uint32_t offset = 0;
		for (uint32_t y = 0; y < yDim; ++y)
		{
			for (uint32_t x = 0; x < xDim; ++x)
			{
				// multiplying by three here because we're dealing with RGB triplets
				offset = NUM_HEADER_BYTES + y * (xDim * 3) + (x * 3);

				pixels[x][y].clusterId = 0;
				pixels[x][y].xPos = x;
				pixels[x][y].yPos = y;
				pixels[x][y].blue = (uint8_t)(image[offset]);
				pixels[x][y].green = (uint8_t)(image[offset + 1]);
				pixels[x][y].red = (uint8_t)(image[offset + 2]);
				pixels[x][y].flag = 0;
			}
		}

		rgbTrip** gpixels = new rgbTrip*[xDim];
		for (uint32_t x = 0; x < xDim; ++x)
		{
			gpixels[x] = new rgbTrip[yDim];
		}

	    DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "pic saved (i ho");
	    DriverStationLCD::GetInstance()->UpdateLCD();
		offset = 0;
		for (uint32_t y = 0; y < yDim; ++y)
		{
			for (uint32_t x = 0; x < xDim; ++x)
			{
				// multiplying by three here because we're dealing with RGB triplets
				offset = NUM_HEADER_BYTES + y * (xDim * 3) + (x * 3);

				gpixels[x][y].clusterId = 0;
				gpixels[x][y].xPos = x;
				gpixels[x][y].yPos = y;
				gpixels[x][y].blue = (uint8_t)(image[offset]);
				gpixels[x][y].green = (uint8_t)(image[offset + 1]);
				gpixels[x][y].red = (uint8_t)(image[offset + 2]);
				gpixels[x][y].flag = 0;
			}
		}


		bool** stat = new bool*[xDim];
		for (uint32_t x = 0; x < xDim; ++x)
		{
			stat[x] = new bool[yDim];
		}
		for (int y = 0; y < yDim; y++){
			for (int x = 0; x < xDim; x++)
			{
				stat[x][y] = 0;
			}
		}

		bool** gstat = new bool*[xDim];
		for (uint32_t x = 0; x < xDim; ++x)
		{
			gstat[x] = new bool[yDim];
		}
		for (int y = 0; y < yDim; y++){
			for (int x = 0; x < xDim; x++)
			{
				gstat[x][y] = 0;
			}
		}

		for (int y = 0; y < yDim; y++)
		{
			for (int x = 0; x < xDim; x++)
			{
				if (pixels[x][y].red > 130 && pixels[x][y].blue > 10 && pixels[x][y].blue < 100 && pixels[x][y].green > 120)
				{
					pixels[x][y].red = 0;
					pixels[x][y].blue = 0;
					pixels[x][y].green = 255;
					stat[x][y] = 1;
				}
				else
				{
					pixels[x][y].red = 0;
					pixels[x][y].blue = 0;
					pixels[x][y].green = 0;
				}
			}
		}
		
	    DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "pic saved (i h");
	    DriverStationLCD::GetInstance()->UpdateLCD();
	    
		// now we have to put our 2D array back into 1D so we can write it out easily
		uint32_t x = 0;
		uint32_t y = 0;
		uint32_t byteCount = 0;
		for (byteCount = 0; byteCount < sizeOfFile - (NUM_HEADER_BYTES + 2); byteCount += 3)
		{
			// dividing by three here because we're dealing with RGB triplets
			x = byteCount / 3 % xDim;
			y = byteCount / xDim / 3;

			image[byteCount + NUM_HEADER_BYTES] = pixels[x][y].blue;
			image[byteCount + NUM_HEADER_BYTES + 1] = pixels[x][y].green;
			image[byteCount + NUM_HEADER_BYTES + 2] = pixels[x][y].red;
		}

		std::ofstream ofl("trueBlue.bmp", std::ios::binary);
		ofl.write((char*)image, sizeOfFile);
		ofl.close();

		// Give our 2D array memory back
		for (uint32_t x = 0; x < xDim; ++x)
		{
			delete[] pixels[x];
		}
		delete[] pixels;

		// Give our 1D array memory back
		delete[] image;


	    DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "pic saved (i");
	    DriverStationLCD::GetInstance()->UpdateLCD();

		///////////////////////////////////////////////////////////////////////////////////////////
		convertToGrayscale(gpixels, xDim, yDim);
		for (int y = 0; y < yDim; y++)
		{
			for (int x = 0; x < xDim; x++)
			{
				if (gpixels[x][y].red > 230 && gpixels[x][y].blue > 230 && gpixels[x][y].green > 230 && y >= 25 && y <= 90)
				{
					gpixels[x][y].red = 255;
					gpixels[x][y].green = 255;
					gpixels[x][y].blue = 255;
					gstat[x][y] = 1;
				}
				else
				{
					gpixels[x][y].red = 0;
					gpixels[x][y].green = 0;
					gpixels[x][y].blue = 0;
				}
			}
		}
		int leftScore = 0;
		int rightScore = 0;
		for (int y = 0; y < yDim; y++){
			for (int x = 0; x < xDim; x++)
			{
				if (gstat[x][y] == 1)
				{
				
					if (x < xDim / 2)
					{
						leftScore++;
					}
					else if (x > xDim / 2)
					{
						rightScore++;
					}
				}
			}
		}
		if (leftScore > rightScore)
			{
				DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "left");
					DriverStationLCD::GetInstance()->UpdateLCD();
					leftOrRight = -1;
					
			}
			else if (rightScore > leftScore)
			{
				DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "right");
					DriverStationLCD::GetInstance()->UpdateLCD();
					leftOrRight = 1;
					
			}
			else
			{
				DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "wut");
					DriverStationLCD::GetInstance()->UpdateLCD();
					
			}
		// now we have to put our 2D array back into 1D so we can write it out easily
		x = 0;
		y = 0;
		byteCount = 0;
		for (byteCount = 0; byteCount < sizeOfFile - (NUM_HEADER_BYTES + 2); byteCount += 3)
		{
			// dividing by three here because we're dealing with RGB triplets
			x = byteCount / 3 % xDim;
			y = byteCount / xDim / 3;

			imagegray[byteCount + NUM_HEADER_BYTES] = gpixels[x][y].blue;
			imagegray[byteCount + NUM_HEADER_BYTES + 1] = gpixels[x][y].green;
			imagegray[byteCount + NUM_HEADER_BYTES + 2] = gpixels[x][y].red;
		}

		std::ofstream oflgray("trueGray.bmp", std::ios::binary);
		oflgray.write((char*)imagegray, sizeOfFilegray);
		oflgray.close();

	    DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "pic saved (");
	    DriverStationLCD::GetInstance()->UpdateLCD();
	    
		// Give our 2D array memory back
		for (uint32_t x = 0; x < xDim; ++x)
		{
			delete[] gpixels[x];
		}
		delete[] gpixels;

		// Give our 1D array memory back
		delete[] imagegray;

	    DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "pic saved");
	    DriverStationLCD::GetInstance()->UpdateLCD();
	    
		return leftOrRight;
	}
	else
		return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
