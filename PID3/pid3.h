#ifndef PID_H
#define PID_H

class PID
{
  private:
    float Kp;
    float Kd;
    float Ki;
    float T;
    float b;
    float K1;
    float K2;
    float k0;
    float LUX_ant;
    float i_ant;
    float e_ant;
    float e;
    float i;
    float p;
    float u;
    float final_u;
    float uff;
  public:
    PID();
    ~PID();
    float control_signal();
};

#endif
