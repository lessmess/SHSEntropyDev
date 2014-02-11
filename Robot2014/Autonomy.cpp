/*
#include "Autonomy.h"
#include "IODefinitions.h"

// IdleState /
IdleState::IdleState()
{
  m_msSpentIdle = 0;
  // NOTE: See here that if this default c'tor is used without changing m_msToSpendIdle,
  // the Idle state will run indefinitely
  m_msToSpendIdle = INT_MAX;
}

IdleState::IdleState(double _timeToSpendIdle)
{
  m_msSpentIdle = 0;
  m_msToSpendIdle = _timeToSpendIdle;
}

void IdleState::setTimeToSpendIdle(double _timeToSpendIdle)
{
  m_msToSpendIdle = _timeToSpendIdle;
}

bool IdleState::Update(double _dt)
{
  // Sum the current time spent idle with the time between this frame and the last frame (delta),
  // and return whether it's exceeded the specified time to spend idle
  return ((m_msSpentIdle += _dt) >= m_msToSpendIdle);
}
PositionState::PositionState()
{
  m_entDrive = 0;
  m_leftEncoder = 0;
  m_rightEncoder = 0;

  m_forwardSpeed = 0.0;
  m_rotation = 0.0;
  m_scalingFactor = 0.0;
  m_deltaLeftDist = 0.0;
  m_deltaRightDist = 0.0;
  m_lastLeftDist = 0.0;
  m_lastRightDist = 0.0;
}

PositionState::~PositionState()
{
  delete m_leftEncoder;
  delete m_rightEncoder;
}

PositionState::PositionState(EntropyDrive& _entDrive, Vec2 _targetPos, double _targetRotation)
{
  m_entDrive = &_entDrive;

  m_leftEncoder = new Encoder(1, 1, 1, 2, false, Encoder::k4X);
  m_rightEncoder = new Encoder(1, 3, 1, 4, true, Encoder::k4X);		 
  
  // TODO: Might not want it in feet
  m_leftEncoder->SetDistancePerPulse(PULSE_RATIO / 12.0);
  m_leftEncoder->SetPIDSourceParameter(Encoder::kRate);
  m_rightEncoder->SetDistancePerPulse(PULSE_RATIO / 12.0);
  m_rightEncoder->SetPIDSourceParameter(Encoder::kRate);
			 
  m_leftEncoder->Start();
  m_rightEncoder->Start();

  m_targetPos = _targetPos;

  m_targetRotation = _targetRotation;
  m_forwardSpeed = 0.03;
  m_rotation = 0.0;
  m_scalingFactor = 0.777;
  m_deltaLeftDist = 0.0;
  m_deltaRightDist = 0.0;
  m_lastLeftDist = 0.0;
  m_lastRightDist = 0.0;
}

bool PositionState::Update(double _dt)
{
  double leftEncDist = m_leftEncoder->GetDistance();
  double rightEncDist = m_rightEncoder->GetDistance();
  // TODO: Shouldn't need to do this
  leftEncDist  *= -1;
  rightEncDist *= -1;

  m_deltaLeftDist = leftEncDist - m_lastLeftDist;
  m_deltaRightDist = rightEncDist - m_lastRightDist;

  if(m_targetRotation)
  {
    m_rotation += (m_deltaLeftDist - m_deltaRightDist) / 2;
    m_forwardSpeed = 0.0;
    m_rotCorrection = 1.0;
  }
  // Get it to drive straight
  else
  {
    m_currentPos.x += m_deltaLeftDist;
    m_currentPos.y += m_deltaRightDist;

    // Add the last rotation correction into this one
    m_rotCorrection = (m_deltaLeftDist - m_deltaRightDist + m_rotCorrection) * m_scalingFactor;
  }
  
  m_entDrive->DriveRobot(m_forwardSpeed, m_rotCorrection);
  	
  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Left Encoder Dist: %f", leftEncDist);
  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line2, "Right Encoder Dist: %f", rightEncDist);
  DriverStationLCD::GetInstance()->UpdateLCD();
  
  m_lastLeftDist = leftEncDist;
  m_lastRightDist = rightEncDist;

  // TODO: This isn't set up for turning so only complete this state if we've driven forward enough
  return (m_currentPos.x > m_targetPos.x || m_rotation > m_targetRotation);
}

bool ShootState::Update(double _dt){ return true; }

Autonomy::Autonomy()
{
}

Autonomy::Autonomy(EntropyDrive& _entDrive)
{
  m_statesToComplete.push_back(new PositionState(_entDrive, Vec2(5,0)));
  // TODO convert to degrees
  m_statesToComplete.push_back(new PositionState(_entDrive, Vec2(0, 0), 2.1));
  m_statesToComplete.push_back(new PositionState(_entDrive, Vec2(5, 0)));

  m_currentState = m_statesToComplete.back();
}

void Autonomy::StartNextState()
{
  delete m_statesToComplete.back();
  m_statesToComplete.pop_back();

  m_currentState = m_statesToComplete.back();
}
  
void Autonomy::Update(double _dt)
{
  if(m_currentState->Update(_dt))
  {
    StartNextState();
  }
}
*/
