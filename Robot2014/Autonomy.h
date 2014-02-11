
 
#ifndef AUTONOMY_H_
#define AUTONOMY_H_

#include "EntropySubsystemTemplate.h"
#include "EntropyDrive.h"
#include <vector>

struct Vec2
{
  // TODO: Param list
  Vec2(){}
  Vec2(double _x, double _y) : x(_x), y(_y) {}
  
  double x;
  double y;
};

class AutonomousState
{
public:
	bool Update(double _dt);
};

class IdleState : public AutonomousState
{
public:
  // Default c'tor will set idle to run indefinitely unless setTimeToSpendIdle mutator is used
  IdleState();
  IdleState(double _timeToSpendIdle);
  
  void setTimeToSpendIdle(double _timeToSpendIdle);

  bool Update(double _dt);
  
private:
  
  // Number of milliseconds spent idle in this state
  double m_msSpentIdle;
  // Number of milliseconds to spend idle before completion of this state
  double m_msToSpendIdle;
};

class PositionState : public AutonomousState
{
public:
  // Default c'tor shouldn't be used. A pointer to the EntropyDrive is needed
  PositionState();
  ~PositionState();
  PositionState(EntropyDrive& _entDrive, Vec2 _targetPos, double _targetRotation = 0.0);

  bool Update(double _dt);
  
private:
  // Pointer to our EntropyDrive
  EntropyDrive* m_entDrive;

  // Left and right encoders
  Encoder* m_leftEncoder;
  Encoder* m_rightEncoder;
  
  // Target position to reach before completion of this state (used in conjunction with m_degToRotate)
  Vec2 m_targetPos;
  // Current position of robot relative to the position in which it began the state
  Vec2 m_currentPos;

  // Number of degrees of rotation required before completion of this state (used in conjunction with m_targetPos)
  double m_targetRotation;
  // Current forward speed
  double m_forwardSpeed;
  // TODO
  double m_rotCorrection;
  // TODO
  double m_rotation;
  // TODO
  double m_scalingFactor;
  // Distance traveled since the last frame
  double m_deltaLeftDist;
  double m_deltaRightDist;
  // Last frame's accumulated distance traveled (using this to calculate deltas)
  double m_lastLeftDist;
  double m_lastRightDist;
};


class ShootState : public AutonomousState
{
public:
  bool Update(double _dt);
};

class Autonomy : public EntropySubsystemTemplate
{
public:
  Autonomy();
  Autonomy(EntropyDrive& _entDrive);
  
  void StartNextState();

  virtual bool Initialize(){ return true; }
  virtual void Cleanup(){}
  virtual char* GetFeedback(){ return "0"; }
	
  void Update(double _dt);
 private:

  AutonomousState* m_currentState;
  std::vector<AutonomousState*> m_statesToComplete;
};

#endif

