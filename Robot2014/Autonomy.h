#ifndef AUTONOMY_H_
#define AUTONOMY_H_

#include "EntropySubsystemTemplate.h"
#include "WPILib.h"
#include "EntropyDrive.h"
#include "IODefinitions.h"
#include <vector>
#include "Gyro.h"



// luminescence of RGB
  const float RED_LUM = 0.3489f;
  const float GREEN_LUM = 0.6570f;
  const float BLUE_LUM = 0.0f;

  const uint32_t NUM_HEADER_BYTES = 54;
struct Vec2
{
  Vec2() : x(0.0), y(0.0) {}
  Vec2(double _x, double _y) : x(_x), y(_y) {}
  
  double x;
  double y;
};

struct rgbTrip
{
	uint32_t clusterId;
	uint32_t xPos;
	uint32_t yPos;

	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t flag;
};

struct cluster
{
	std::vector<rgbTrip> pixels;

	uint32_t id;
};

class AutonomousState
{
public:
  virtual bool Update(double _dt) = 0;
  virtual void Init() = 0;
};

class IdleState: public AutonomousState
{
public:
  // Default c'tor will set idle to run indefinitely unless setTimeToSpendIdle mutator is used
  IdleState();
  IdleState(EntropyDrive& _entDrive, double _timeToSpendIdle);
  
  void setTimeToSpendIdle(double _timeToSpendIdle);

  bool Update(double _dt);
  void Init() {}
  
private:

  // Pointer to our EntropyDrive
  EntropyDrive* m_entDrive;
  
  // Number of milliseconds spent idle in this state
  double m_msSpentIdle;
  // Number of milliseconds to spend idle before completion of this state
  double m_msToSpendIdle;
};

class DecisionState: public AutonomousState
{
public:
  // Default c'tor will set idle to run indefinitely unless setTimeToSpendIdle mutator is used
  DecisionState();
  DecisionState(EntropyDrive& _entDrive);

  bool Update(double _dt);
  void Init() {}
  int Camera();
    uint32_t readFileSize(std::ifstream& _ifl);
    uint32_t readDimension(std::ifstream& _ifl, uint32_t _offset);
    void convertToGrayscale(rgbTrip**& _pixArray, uint32_t _xSize, uint32_t _ySize);
  
private:

  // Pointer to our EntropyDrive
  EntropyDrive* m_entDrive;
  AxisCamera* camera;
    int leftOrRight;
};

class PositionState : public AutonomousState
{
public:
  // Default c'tor shouldn't be used. A pointer to the EntropyDrive is needed.
  PositionState();
  PositionState(EntropyDrive& _entDrive, Vec2 _targetPos, Encoder* _leftEncoder, Encoder* _rightEncoder, double _targetRotation = 0.0);

  bool Update(double _dt);
  void Init();
 
  
private:
  // Pointer to our EntropyDrive
  EntropyDrive* m_entDrive;

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
  
};

class RotationState : public AutonomousState
{
public:
  // Default c'tor shouldn't be used. A pointer to the EntropyDrive is needed.
  RotationState();
  RotationState(EntropyDrive& _entDrive, Encoder* _leftEncoder, Encoder* _rightEncoder, double _targetRotation, Gyro *_Gyro);

  bool Update(double _dt);
  void Init();
  
private:
  // Pointer to our EntropyDrive
  EntropyDrive* m_entDrive;

  Encoder* m_leftEncoder;
  Encoder* m_rightEncoder;
  
  // Number of degrees of rotation required before completion of this state (used in conjunction with m_targetPos)
  double m_targetRotation;
  double m_rotation;
  // Distance traveled since the last frame
  double m_deltaLeftDist;
  double m_deltaRightDist;
  Gyro *m_Gyro;
};

class ShootState : public AutonomousState
{
public:
  bool Update(double _dt);
  void Init() {}
};

class Autonomy : public EntropySubsystemTemplate
{
public:
  Autonomy();
  Autonomy(EntropyDrive& _entDrive, Gyro *_Gyro);

  void StartNextState();

  virtual bool Initialize(){ return true; }
  virtual void Cleanup(){}
  virtual char* GetFeedback(){ return "x"; }
	
  void Update(double _dt);
  Gyro *m_Gyro;
 
  
 private:
  AutonomousState* m_currentState;
  std::vector<AutonomousState*> m_statesToComplete;
  Encoder* m_leftEncoder;
  Encoder* m_rightEncoder;
  AnalogChannel* m_analogChan;
 
};

#endif
