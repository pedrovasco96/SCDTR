#include "pid3.h"

using namespace std;

PID::PID() {
  Kp = 0.1419;
  Ki = 606.06;
  T = 0.025;
  b = 0.2; //has values between 0 and 1, should be step experimentally to reduce overshoot
  K1 = Kp * b;
  K2 = Kp * Ki * T / 2;
  k0 = 7;
  e = 0;
  p = 0;
  i = 0;
  u = 0;
  final_u = 0;
  uff = 0;
  LUX_ant = 0;
  i_ant = 0;
  e_ant = 0;
}

PID::~PID() {
}

float PID::control_signal(float ref, float LUX)
{
  // defines error
  e = ref - LUX;

  if (ref_change == 1) {
    Serial.println("consensus");
    uff = consensus_function();
    uff = uff / 100 * 255;
    //uff = k0*ref;
    ref_change = 0;
  }
  else {
    // proporcional control
    p = K1 * ref - Kp * LUX;

    // integral control
    i = i_ant + K2 * (e + e_ant);

    //Anti-WindUp
    i = constrain(i, -uff - 5, 255 - uff + 5);
  }


  u = p + i + uff;

  //deadzone
  if  ( u >=  2)
    final_u = u - 2;
  else  if  (u  <=  -2)
    final_u = u - 2;
  else
    final_u = 0;


  /*  Serial.print("i:");
    Serial.print(i);
    Serial.print(" u:");
    Serial.print(u);
    Serial.print(" "); */

  i_ant = i;
  e_ant = e;

  return final_u;
}

float compute_func (float rho, float d_av[], float d[], float y[] )
{
  float min_unconstrained = 0.5 * qn * pow((d[node]), 2) + cn * d[node];
  for (int i = 0; i < N; i++)
    min_unconstrained += y[i] * (d[i] - d_av[i]) + (rho / 2) * pow((d[i] - d_av[i]), 2);
  return min_unconstrained;
}

float consensus_function()
{
  float rho = 0.01;
  int iter = 1; int i;
  float d_av[N] = {0}; float d_best[N] = {0}; float y[N] = {0};
  int min_best_1[50] = {0};
  int sol_unconstrained; int sol_boundary_linear;
  int sol_boundary_0; int sol_boundary_100;
  int sol_linear_0; int sol_linear_100;
  float z[N];
  float u1 = on - Ln; int u2 = 0; int u3 = 100;
  float p[N]; float n;
  float w1 = 0; float w2; float w3; float dot;
  float dunc[N]; float dlinb[N]; float db0[N]; float db100[N];
  float dblin0[N]; float dblin100[N];
  float min_unconstrained;
  float min_boundary_linear; float min_boundary_0; float min_boundary_100;
  float common; float min_linear_0; float min_linear_100;
  float det1; float det2; float det3; float det4;
  float x1; float x2;
  float v1; float v2;
  float dlin0[N]; float dlin100[N];

  for (iter = 0; iter < 50; iter++)
  {
    min_best_1[iter] = 10000; /*big number*/
    sol_unconstrained = 1;
    sol_boundary_linear = 1;
    sol_boundary_0 = 1;
    sol_boundary_100 = 1;
    sol_linear_0 = 1;
    sol_linear_100 = 1;
    u1 = on - Ln;
    u2 = 0;
    u3 = 100;
    w1 = 0;
    n = 0; dot = 0;
    for (i = 0; i < N; i++)
    {
      d_best[i] = -1;
      z[i] = -y[i] + rho * d_av[i];
      p[i] = 1 / rho;
      if (i == node)
      {
        z[i] -= cn;
        p[i] = 1 / (rho + qn);
      }
      n += Kn[i] * Kn[i] * p[i];
      w1 -= Kn[i] * p[i] * z[i];
      dunc[i] = p[i] * z[i];
      dot += dunc[i] * Kn[i];
    }
    w2 = -z[node] * p[node];
    w3 = -w2;
    /*check feasibility of unconstrained minimum using local constraints*/
    if (dunc[node] < 0 || dunc[node] > 100 || dot < -u1)
      sol_unconstrained = 0;
    /*compute minimum constrained to linear boundary   */
    for (i = 0; i < N; i++)
      dlinb[i] = p[i] * (z[i] + Kn[i] / n * (w1 - u1));

    /*check feasibility of minimum constrained to linear boundary*/
    if (dlinb[node] < 0 || dlinb[node] > 100)
      sol_boundary_linear = 0;
    /*compute minimum constrained to 0 boundary*/
    dot = 0;
    for (i = 0; i < N; i++)
    {
      if (i != node)
        db0[i] = p[i] * z[i];
      else
        db0[i] = 0;
      dot += Kn[i] * db0[i];
    }
    /*check feasibility of minimum constrained to 0 boundary*/
    if (db0[node] > 100 || dot < -u1)
      sol_boundary_0 = 0;
    /*compute minimum constrained to 100 boundary*/
    for (i = 0; i < N; i++)
      db100[i] = db0[i];
    db100[node] = 100;
    dot += 100 * Kn[node];
    /*check feasibility of minimum constrained to 100 boundary*/
    if (db0 < 0 || dot < -u1)
      sol_boundary_100 = 0;
    /* compute minimum constrained to linear and zero boundary */
    common = (rho + qn) / ((rho + qn) * n - Kn[node] * Kn[node]);
    det1 = common;
    det2 = -Kn[node] * common;
    det3 = det2;
    det4 = n * (rho + qn) * common;
    x1 = det1 * w1 + det2 * w2;
    x2 = det3 * w1 + det4 * w2;
    v1 = det1 * u1;
    v2 = det3 * u1;
    for (i = 0; i < N; i++)
    {
      dblin0[i] = p[i] * z[i] + p[i] * Kn[i] * (x1 - v1);
      if (i == node)
        dblin0[i] += p[i] * (x2 - v2);
    }
    /*check feasibility*/
    if (dblin0[node] > 100)
      sol_linear_0 = 0;
    /* compute minimum constrained to linear and 100 boundary*/
    common = (rho + qn) / ((rho + qn) * n - Kn[node] * Kn[node]);
    det1 = common;
    det2 = Kn[node] * common;
    det3 = det2;
    det4 = n * (rho + qn) * common;
    x1 = det1 * w1 + det2 * w3;
    x2 = det3 * w1 + det4 * w3;
    v1 = det1 * u1 + det2 * u3;
    v2 = det3 * u1 + det4 * u3;
    for (i = 0; i < N; i++)
    {
      dblin100[i] = p[i] * z[i] + p[i] * Kn[i] * (x1 - v1);
      if (i == node)
        dblin100[i] -= p[i] * (x2 - v2);
    }
    /*check feasibility*/
    if (dblin100[node] < 0)
      sol_linear_100 = 0;
    /*compute function value and if best store new optimum*/
    if (sol_unconstrained)
    {
      min_unconstrained = compute_func (rho, d_av, dunc, y);
      if (min_unconstrained < min_best_1[iter])
      {
        for (i = 0; i < N; i++)
          d_best[i] = dunc[i];
        min_best_1[iter] = min_unconstrained;
      }
    }
    /*compute function value and if best store new optimum*/
    if (sol_boundary_linear)
    {
      min_boundary_linear = compute_func (rho, d_av, dlinb, y);
      if (min_boundary_linear < min_best_1[iter])
      {
        for (i = 0; i < N; i++)
          d_best[i] = dlinb[i];
        min_best_1[iter] = min_boundary_linear;
      }
    }
    /* compute function value and if best store new optimum*/
    if (sol_boundary_0)
    {
      min_boundary_0 = compute_func (rho, d_av, db0, y);
      if (min_boundary_0 < min_best_1[iter])
      {
        for (i = 0; i < N; i++)
          d_best[i] = db0[i];
        min_best_1[iter] = min_boundary_0;
      }
    }
    /* compute function value and if best store new optimum*/
    if (sol_boundary_100)
    {
      min_boundary_100 = compute_func (rho, d_av, db100, y);
      if (min_boundary_100 < min_best_1[iter])
      {
        for (i = 0; i < N; i++)
          d_best[i] = db100[i];
        min_best_1[iter] = min_boundary_100;
      }
    }
    /* compute function value and if best store new optimum*/
    if (sol_linear_0)
    {
      min_linear_0 = compute_func (rho, d_av, dblin0, y);
      if (min_linear_0 < min_best_1[iter])
      {
        for (i = 0; i < N; i++)
          d_best[i] = dblin0[i];
        min_best_1[iter] = min_linear_0;
      }
    }
    /* compute function value and if best store new optimum*/
    if (sol_linear_100)
    {
      min_linear_100 = compute_func (rho, d_av, dblin100, y);
      if (min_linear_100 < min_best_1[iter])
      {
        for (i = 0; i < N; i++)
          d_best[i] = dblin100[i];
        min_best_1[iter] = min_linear_100;
      }
    }

    // Serial.println("Ready to share");

    for (i = 0; i < N; i++) {
      for (int sending = 0; sending < N; sending++) {
        if (sending == node) {
          n_done = 0;
          Wire.beginTransmission(0);
          Wire.write('c');
          //Serial.println("Sent c");
          aux = int(d_best[i]);
          Wire.write(aux);
          // Serial.println("Sent d_best");
          Wire.endTransmission();

          start_time = millis();
        }
        else {
          while (n_drec < 1) {
            delay(1);
          }
          Wire.beginTransmission(sending);
          Wire.write('b');
          Wire.endTransmission();
          n_drec = 0;
        }
      }
      d_av[i] = (d_rec + d_best[i]) / N;
      y[i] = y[i] + rho * (d_best[i] - d_av[i]);
      d_rec = 0;
    }

    //Serial.print("Finished iteration ");
    //Serial.println(iter);
  }
  return d_best[node];

}
