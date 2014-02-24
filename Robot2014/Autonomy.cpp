#include "Autonomy.h"

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
  return false;
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
  m_forwardSpeed = -0.6;
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
  
  m_entDrive->DriveRobot(m_forwardSpeed, m_rotCorrection);

  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Positioning");
  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line4, "Rotation Auto: %f", m_rotCorrection);
  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line5, "Left Dist: %f", m_leftEncoder->GetDistance());
  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line6, "Right Dist: %f", m_rightEncoder->GetDistance());
  DriverStationLCD::GetInstance()->UpdateLCD();
  
  s_lastLeftDist = leftEncDist;
  s_lastRightDist = rightEncDist;

  return ((m_currentPos.x + m_currentPos.y) / 2) * -1 > m_targetPos.x;
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

RotationState::RotationState(EntropyDrive& _entDrive, Encoder* _leftEncoder, Encoder* _rightEncoder,  double _targetRotation)
{
  m_entDrive = &_entDrive;	 

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
  double leftEncDist = m_leftEncoder->GetDistance();
  //double rightEncDist = m_rightEncoder->GetDistance();
  leftEncDist *= -1.0;

  m_deltaLeftDist = leftEncDist - s_lastLeftDist;
  //m_deltaRightDist = rightEncDist - s_lastRightDist;
  
  m_rotation += (m_deltaLeftDist);// - m_deltaRightDist);

  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Rotating");
  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line4, "Last Dist: %f", s_lastLeftDist);
  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line5, "Left Dist: %f", leftEncDist);
  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line6, "Rotation: %f", m_rotation);
  DriverStationLCD::GetInstance()->UpdateLCD();
    
  m_entDrive->DriveRobot(0, 1);
  	
  s_lastLeftDist = leftEncDist;
  //s_lastRightDist = rightEncDist;

  return m_rotation * -1.0 > m_targetRotation;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////// ShootState ///////////////////////////////////////////
bool ShootState::Update(double _dt){ return true; }

///////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////// Autonomy /////////////////////////////////////////////
Autonomy::Autonomy()
{
}

Autonomy::Autonomy(EntropyDrive& _entDrive)
{
	// TODO: Might not want it in feet
	m_leftEncoder = new Encoder(1, 1, 1, 2, false, Encoder::k4X);
	m_rightEncoder = new Encoder(1, 3, 1, 4, true, Encoder::k4X);
	
	m_leftEncoder->SetDistancePerPulse(PULSE_RATIO / 12.0);
	m_leftEncoder->SetPIDSourceParameter(Encoder::kRate);
	m_rightEncoder->SetDistancePerPulse(PULSE_RATIO / 12.0);
	m_rightEncoder->SetPIDSourceParameter(Encoder::kRate);
					 
	m_leftEncoder->Start();
	m_rightEncoder->Start();
	
  //m_statesToComplete.push_back(new RotationState(_entDrive, 3.5));
  //m_statesToComplete.push_back(new PositionState(_entDrive, Vec2(4, 0)));
  //m_statesToComplete.push_back(new RotationState(_entDrive, 3.5));
  //m_statesToComplete.push_back(new PositionState(_entDrive, Vec2(2, 0), m_leftEncoder, m_rightEncoder));
  //m_statesToComplete.push_back(new RotationState(_entDrive, m_leftEncoder, m_rightEncoder, 2.0));
  //m_statesToComplete.push_back(new PositionState(_entDrive, Vec2(2, 0), m_leftEncoder, m_rightEncoder));
  //m_statesToComplete.push_back(new RotationState(_entDrive, m_leftEncoder, m_rightEncoder, 3.1));
  m_statesToComplete.push_back(new IdleState(_entDrive, 2000000000));
  m_statesToComplete.push_back(new PositionState(_entDrive, Vec2(8, 0), m_leftEncoder, m_rightEncoder));

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

///////////////////////////////////////////////////////////////////////////////////////////////////
