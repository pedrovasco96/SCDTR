float compute_func (float qn, int cn, float rho, float d_av[], float d[], int N)
{
  float min_unconstrained = 0.5*qn*(dunc[node])^2 + cn*dunc[node];
  for (i = 0; i < N; i++)
    min_unconstrained += y[i]*(d[i]-d_av[i])+(rho/2)*(d[i]-d_av[i])^2;
  return min_unconstrained;
}

float[] consensus_function(float dn[], int node, float cn, float qn, float Kn[], float on, float Ln, int N)
{
  float rho = 0.01;
  int iter = 1; int i;
  float d_av[N]={0}; float d_in=dn; float d_best[N]; float y[N] = {0};
  int min_best_1[50] = {0};
  int sol_unconstrained; int sol_boundary_linear;
  int sol_boundary_0; int sol_boundary_100;
  int sol_linear_0; int sol_linear_100;
  float z[N];
  float u1=on-Ln; int u2=0; int u3=100;
  float p[N]; float n;
  float w1; float w2; float w3; float dot;
  float dunc[N]; float dlinb[N]; float db0[N]; float db100[N];
  float min_unconstrained;
  float min_boundary_linear; float min_boundary_0; float min_boundary_100;
  float common; float min_linear_0, float min_linear_100;
  float det1; float det2; float det3; float det4;
  float x1; float x2;
  float v1; float v2;
  float dlin0[N]; float dlin100[N];
  for (i = 0; i < N; i++)
  {
    d_av[0]+=dn[i];
  }
  for (i = 1; i < N; i++)
  {
    d_av[i]=dn[1];
  }
  for (iter = 1; iter <= 50; iter++)
  {
     min_best_1(iter) = 100000; /*big number*/
     sol_unconstrained = 1;
     sol_boundary_linear = 1;
     sol_boundary_0 = 1;
     sol_boundary_100 = 1;
     sol_linear_0 = 1;
     sol_linear_100 = 1;
     u1 = on-Ln;
     u2 = 0;
     u3 = 100;
     n=0; w=0; dot=0;
     for (i = 0; i < N; i++)
     {  
       d_best[i]=-1;
       z[i]=-y[i]+rho*d_av[i];
       p[i]=1/rho;
       if (i=node)
       {
        z[i]-= cn;
        p[i]=1/(rho+qn); 
       }
       n += Kn[i]*Kn[i]*p[i];
       w1 -=Kn[i]*p[i]*z[i];
       dunc[i]=p[i]*z[i];
       dot += dunc[i]*Kn[i];
     }
     w2 = -z[node]*p[node];
     w3 = -w2;
     /*check feasibility of unconstrained minimum using local constraints*/
     if (dunc[node] < 0 || dunc[node] > 100 || dot < -u1)
       sol_unconstrained = 0;
     /*compute minimum constrained to linear boundary   */
     for (i = 0; i < N; i++)
      dlinb[i]=p[i]*(z[i]+Kn[i]/n*(w1-u1);
     /*check feasibility of minimum constrained to linear boundary*/
     if (dlinb[node] < 0 || dlinb[node] > 100)
      sol_boundary_linear = 0;
     /*compute minimum constrained to 0 boundary*/
     dot=0;
     for (i = 0; i < N; i++)
     {
      if(i != node)
        db0[i]=p[i]*z[i];
      else
        db0[i]=0;
      dot +=Kn[i]*db0[i];
     }
     /*check feasibility of minimum constrained to 0 boundary*/
     if (db0[node] > 100 || dot < -u1)
      sol_boundary_0 = 0;
     /*compute minimum constrained to 100 boundary*/
     for (i = 0; i < N; i++)
      db100[i]=db0[i];
     db100[node]=100;
     dot+=100;
     /*check feasibility of minimum constrained to 100 boundary*/
     if (db0 < 0 || dot < -u1)
      sol_boundary_100 = 0;
     /* compute minimum constrained to linear and zero boundary */
     common = (rho+qn)/((rho+qn)*n-Kn[node]*Kn[node]);
     det1 = common;
     det2 = -Kn[node]*common;
     det3 = det2;
     det4 = n*(rho+qn)*common;
     x1 = det1*w1 + det2*w2;
     x2 = det3*w1 + det4*w2;
     v1 = det1*u1;
     v2 = det3*u1;
     for (i = 0; i < N; i++)
     {
      dblin0[i]=p[i]*z[i]+p[i]*Kn[i]*(x1-v1);
      if (i = node)
        dblin0[i] += p[i]*(x2-v2);  
     }
     /*check feasibility*/
     if (dblin0[node] > 100) 
      sol_linear_0 = 0;
     /* compute minimum constrained to linear and 100 boundary*/
     common = (rho+qn)/((rho+qn)*n-Kn[node]*Kn[node]);
     det1 = common;
     det2 = Kn[node]*common;
     det3 = det2;
     det4 = n*(rho+qn)*common;
     x1 = det1*w1 + det2*w3;
     x2 = det3*w1 + det4*w3;
     v1 = det1*u1 + det2*u3; 
     v2 = det3*u1 + det4*u3;
     for (i = 0; i < N; i++)
     {
      dblin100[i]=p[i]*z[i]+p[i]*Kn[i]*(x1-v1);
      if (i = node)
        dblin100[i] -= p[i]*(x2-v2);  
     }
     /*check feasibility*/
     if (dblin100[node] < 0)
      sol_linear_100 = 0;
     /*compute function value and if best store new optimum*/
     if (sol_unconstrained)
     {
         min_unconstrained = compute_func (qn, cn, rho, d_av, dunc, N);
         if (min_unconstrained < min_best_1[iter])
         {
             for (i = 0; i < N; i++)
              d_best[iter]=dunc[i];
             min_best_1[iter] = min_unconstrained;
         }
     }
     /*compute function value and if best store new optimum*/
     if (sol_boundary_linear)
     {
        min_boundary_linear = compute_func (qn, cn, rho, d_av, dlinb, N);
        if (min_boundary_linear < min_best_1[iter])
        {
             for (i = 0; i < N; i++)
              d_best[iter]=dlinb[i];
             min_best_1[iter] = min_boundary_linear;
        }
     }
     /* compute function value and if best store new optimum*/
     if (sol_boundary_0)
     { 
          min_boundary_0 = compute_func (qn, cn, rho, d_av, db0, N);
         if (min_boundary_0 < min_best_1[iter])
         {
             for (i = 0; i < N; i++)
              d_best[iter]=db0[i];
             min_best_1[iter] = min_boundary_0;
         }
     }
     /* compute function value and if best store new optimum*/
     if (sol_boundary_100)
     { 
         min_boundary_100 = compute_func (qn, cn, rho, d_av, db100, N);
         if (min_boundary_100 < min_best_1[iter])
             for (i = 0; i < N; i++)
              d_best[iter]=db100[i];
             min_best_1[iter] = min_boundary_100;
         end;
     end;
     /* compute function value and if best store new optimum*/
     if (sol_linear_0) 
     { 
         min_linear_0 = compute_func (qn, cn, rho, d_av, dblin0, N);
         if (min_linear_0 < min_best_1[iter])
         {
             for (i = 0; i < N; i++)
              d_best[iter]=dblin0[i];
             min_best_1[iter] = min_linear_0;
         }
     }
     /* compute function value and if best store new optimum*/
     if (sol_linear_100)
     { 
          min_linear_100 = compute_func (qn, cn, rho, d_av, dblin100, N);
         if (min_linear_100 < min_best_1[iter])
         {
             for (i = 0; i < N; i++)
              d_best[iter]=dblin100[i];
             min_best_1[iter] = min_linear_100;
         }
     }
     /*store data and save for next cycle
     best_d11(i) = d11_best;
     best_d12(i) = d12_best;
     d1 = [d11_best;d12_best];*/
  }
}

