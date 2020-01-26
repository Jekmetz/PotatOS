/*This file will serve as  the prototype list
for commands.c*/
#define SUCCESS 0

/*
  Procedure: cmd_help
  Params: params-will serve as the params for each of these things
*/
int cmd_help(char* params);

int cmd_version(char* params);

int cmd_shutdown(char* params);

int cmd_date(char* params);

int cmd_time(char* params);

int cmd_test(char* params);

int set_flags(char** gparams, int * flag, int pcount);

int set_gparams(char* params, int* pcount);

// cmd_help flags
#define A_FLAG (1 << 0)
#define B_FLAG (1 << 1)
#define C_FLAG (1 << 2)
#define D_FLAG (1 << 3)
#define E_FLAG (1 << 4)
#define F_FLAG (1 << 5)
#define G_FLAG (1 << 6)
#define H_FLAG (1 << 7)
#define I_FLAG (1 << 8)
#define J_FLAG (1 << 9)
#define K_FLAG (1 << 10)
#define L_FLAG (1 << 11)
#define M_FLAG (1 << 12)
#define N_FLAG (1 << 13)
#define O_FLAG (1 << 14)
#define P_FLAG (1 << 15)
#define Q_FLAG (1 << 16)
#define R_FLAG (1 << 17)
#define S_FLAG (1 << 18)
#define T_FLAG (1 << 19)
#define U_FLAG (1 << 20)
#define V_FLAG (1 << 21)
#define W_FLAG (1 << 22)
#define Y_FLAG (1 << 23)
#define X_FLAG (1 << 24)
#define Z_FLAG (1 << 25)
