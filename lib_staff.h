

// Структура Staff
typedef struct {
    int id;
    char name[100];
    int passport;
    int snils;
    char position[50];
    int salary;
    char status[100];
    char emp_date[12];
    int phone_num;
    int inn;
} Staff;

// Структура StaffHeadquarters
typedef struct {
    Staff* staff[1000];
    int quantity;
} StaffHeadquarters;
