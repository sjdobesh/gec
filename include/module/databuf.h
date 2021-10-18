#ifndef _DATABUF_H_
#define _DATABUF_H_

#include <stdlib.h>

// ENUMS //---------------------------------------------------------------------

enum db_type {
  INT  = (1u << 0),
  FLT  = (1u << 1),
  DBL  = (1u << 2),
  CHR  = (1u << 3),
  INTP = (1u << 4),
  FLTP = (1u << 5),
  DBLP = (1u << 6),
  CHRP = (1u << 7),
  VOID = (1u << 8)
};

// STRUCTS //-------------------------------------------------------------------

typedef struct data {
  void* ptr;
  unsigned int type;
  unsigned int size;
  char* name;
  int id;
} data;

typedef struct databuf {
  data* d;
  int n;
  int next_id;
} databuf;

int get_next_id(databuf*);
int get_first_id(databuf);
int get_last_id(databuf);

void print_type(int type);
void print_databuf(databuf db);
void printv_databuf(databuf db);
void print_var_id(databuf db, int);
void print_var_name(databuf db, char*);
#define print_var(databuf, id) _Generic ((id), \
int: print_var_id, char*: print_var_name)(databuf, id)

databuf create_databuf();
void free_databuf(databuf*);

// ADD TO BUFFER
int add_data(databuf*, data);
data wrap_int (int*,     char*);
data wrap_flt (float*,   char*);
data wrap_dbl (double*,  char*);
data wrap_chr (char*,    char*);
data wrap_intp(int**,    char*);
data wrap_fltp(float**,  char*);
data wrap_dblp(double**, char*);
data wrap_chrp(char**,   char*);

#define wrap_data(data, name) _Generic ((data), \
char*:   wrap_chr, char**:   wrap_chrp,   \
int*:    wrap_int, int**:    wrap_intp,   \
float*:  wrap_flt, float**:  wrap_fltp,   \
double*: wrap_dbl, double**: wrap_dblp   \
)(data, name)

int new_int (databuf*, char*, int);
int new_flt (databuf*, char*, float);
int new_dbl (databuf*, char*, double);
int new_chr (databuf*, char*, char);
int new_intp(databuf*, char*, int*);
int new_fltp(databuf*, char*, float*);
int new_dblp(databuf*, char*, double*);
int new_chrp(databuf*, char*, char*);

#define new_var(db, name, x) _Generic ((x), \
char:   new_chr, char*:   new_chrp,\
int:    new_int, int*:    new_intp,\
float:  new_flt, float*:  new_fltp,\
double: new_dbl, double*: new_dblp\
)(db, name, x)

// GET FROM BUFFER
int get_data_id(databuf*, int, data*);
int get_data_name(databuf*, char*, data*);
#define get_data(databuf, id, data) _Generic ((id), \
int: get_data_id, char*: get_data_name)(databuf, id, data)

// int
int get_int_id   (databuf, int,   int*);
int get_int_name (databuf, char*, int*);
int get_intp_id  (databuf, int,   int**);
int get_intp_name(databuf, char*, int**);
// float
int get_flt_id   (databuf, int,   float*);
int get_flt_name (databuf, char*, float*);
int get_fltp_id  (databuf, int,   float**);
int get_fltp_name(databuf, char*, float**);
// double
int get_dbl_id   (databuf, int,   double*);
int get_dbl_name (databuf, char*, double*);
int get_dblp_id  (databuf, int,   double**);
int get_dblp_name(databuf, char*, double**);
// CHAR
int get_chr_id   (databuf, int,   char*);
int get_chr_name (databuf, char*, char*);
int get_chrp_id  (databuf, int,   char**);
int get_chrp_name(databuf, char*, char**);

#define get_var_id(var) _Generic ((var), \
char*:   get_chr_id, char**:   get_chrp_id,\
int*:    get_int_id, int**:    get_intp_id,\
float*:  get_flt_id, float**:  get_fltp_id,\
double*: get_dbl_id, double**: get_dblp_id\
)
#define get_var_name(var) _Generic ((var), \
char*:   get_chr_name, char**:   get_chrp_name,\
int*:    get_int_name, int**:    get_intp_name,\
float*:  get_flt_name, float**:  get_fltp_name,\
double*: get_dbl_name, double**: get_dblp_name\
)
#define get_var(databuf, id, var) _Generic ((id), \
int: get_var_id(var), char*: get_var_name(var))(databuf, id, var)

// UPDATE VARIABLES
int set_int_id   (databuf*, int,   int);
int set_int_name (databuf*, char*, int);
int set_intp_id  (databuf*, int,   int*);
int set_intp_name(databuf*, char*, int*);
// float
int set_flt_id   (databuf*, int,   float);
int set_flt_name (databuf*, char*, float);
int set_fltp_id  (databuf*, int,   float*);
int set_fltp_name(databuf*, char*, float*);
// double
int set_dbl_id   (databuf*, int,   double);
int set_dbl_name (databuf*, char*, double);
int set_dblp_id  (databuf*, int,   double*);
int set_dblp_name(databuf*, char*, double*);
// CHAR
int set_chr_id   (databuf*, int,   char);
int set_chr_name (databuf*, char*, char);
int set_chrp_id  (databuf*, int,   char*);
int set_chrp_name(databuf*, char*, char*);

#define set_var_id(var) _Generic ((var), \
char:   set_chr_id, char*:   set_chrp_id,\
int:    set_int_id, int*:    set_intp_id,\
float:  set_flt_id, float*:  set_fltp_id,\
double: set_dbl_id, double*: set_dblp_id\
)
#define set_var_name(var) _Generic ((var), \
char:   set_chr_name, char*:   set_chrp_name,\
int:    set_int_name, int*:    set_intp_name,\
float:  set_flt_name, float*:  set_fltp_name,\
double: set_dbl_name, double*: set_dblp_name\
)
#define set_var(databuf, id, var) _Generic ((id), \
int: set_var_id(var), char*: set_var_name(var))(databuf, id, var)

// FREE DATA
int free_var_id(databuf*, int);
int free_var_name(databuf*, char*);
#define free_var(databuf, id) _Generic ((id), \
int: free_var_id, char*: free_var_name)(databuf, id)

#endif
