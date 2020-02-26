/**
* @file commandUtils.h
*
* @brief Utilites that apply to all command files
*/
#pragma once 

/**
* @brief The command input buffer
*
* This a macro to store the command input buffer. Here we can change
* the amount of characters we allow to be entered into the command
* handler at once. We currently allow 100 characters.
*/
#define CMDSIZE 100

/**
* @brief Macro to return a 0 on success
*/
#define SUCCESS 1
/**
* @brief Macro to return a -1 on failure
*/
#define FAILURE 0
/**
* @brief The maximum parameters allowed per command
*/
#define MAXPARAMCOUNT 10

/** cmd_help flags */
/** A flag binary bit shift macro */
#define A_FLAG (1 << 0)
/** B flag binary bit shift macro */
#define B_FLAG (1 << 1)
/** C flag binary bit shift macro */
#define C_FLAG (1 << 2)
/** D flag binary bit shift macro */
#define D_FLAG (1 << 3)
/** E flag binary bit shift macro */
#define E_FLAG (1 << 4)
/** F flag binary bit shift macro */
#define F_FLAG (1 << 5)
/** G flag binary bit shift macro */
#define G_FLAG (1 << 6)
/** H flag binary bit shift macro */
#define H_FLAG (1 << 7)
/** I flag binary bit shift macro */
#define I_FLAG (1 << 8)
/** J flag binary bit shift macro */
#define J_FLAG (1 << 9)
/** K flag binary bit shift macro */
#define K_FLAG (1 << 10)
/** L flag binary bit shift macro */
#define L_FLAG (1 << 11)
/** M flag binary bit shift macro */
#define M_FLAG (1 << 12)
/** N flag binary bit shift macro */
#define N_FLAG (1 << 13)
/** O flag binary bit shift macro */
#define O_FLAG (1 << 14)
/** P flag binary bit shift macro */
#define P_FLAG (1 << 15)
/** Q flag binary bit shift macro */
#define Q_FLAG (1 << 16)
/** R flag binary bit shift macro */
#define R_FLAG (1 << 17)
/** S flag binary bit shift macro */
#define S_FLAG (1 << 18)
/** T flag binary bit shift macro */
#define T_FLAG (1 << 19)
/** U flag binary bit shift macro */
#define U_FLAG (1 << 20)
/** V flag binary bit shift macro */
#define V_FLAG (1 << 21)
/** W flag binary bit shift macro */
#define W_FLAG (1 << 22)
/** Y flag binary bit shift macro */
#define Y_FLAG (1 << 23)
/** X flag binary bit shift macro */
#define X_FLAG (1 << 24)
/** Z flag binary bit shift macro */
#define Z_FLAG (1 << 25)
/** NO flag binary bit shift macro */
#define NO_FLAG (1<<26)
/** A flag binary bit shift macro */

/**
* @brief A string to hold the command input up to the max command size
*/
extern char gparamstr[CMDSIZE];
/**
* @brief Will hold all the string pointers
*/
extern char* gparams[27];  // will hold all of the parameters

/**
* @brief A helper macro that will take a letter and return its integer equivalent
*
* This is a helper macro that is used in set_flags and get_gparams. It takes in
* character and return the integer equivalent of that character.
*
* @param c The character to be returned as an int
*/
#define alphanum(c) (('a' <= c && c <= 'z') ? c - 'a' : c - 'A')

/**
* @brief A struct to hold command aliases
*
* The ALIAS Struct is a custom struct that is designed to hold aliases for commands
*
* @param c A string that will hold the initial command name
* @param val A string pointer that will point to the original command name
*/
typedef struct {
  char c;
  char* val;
} ALIAS;

/**
* @brief Sets flags based on param string, flags and num aliases
*
* Usage:
*    set_flags(paramstr,&flag,5,
*    'a',"alpha",
*    'b',"bravo",
*    'f',"foxtrot",
*    'g',"golf",
*    'r',"whiskey"
*    )
*
* @param paramstr string that each command gets. Typed by the user
* @param flag pointer to integer flag
* @param num_aliases number of aliases specified
*
* @return success or failure
*
* @note num_aliases must be the exact number of parameters. In the example, 5
*/
int set_flags(char* paramstr, int* flag, int num_aliases, ...);

/**
* @brief Gets value of specific flag
*
* Usage: get_pvalue('a');
*
* @param c character of flag to get the value from
*
* @return value after the flag specified
*/
char* get_pvalue(char c);

/**
* @brief Used as a helper function for set_flags
*
* @param alias alias to search for in aliases
* @param num_aliases number of aliases in aliases
* @param aliases array of ALIASes to search through
*
* @return character of flag that it found
*/
char set_flags_search_alias(char* alias, int num_aliases, ALIAS aliases[]);