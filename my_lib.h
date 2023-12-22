#define MAX_DESCRIPTION_LENGTH 100
#define MAX_AMOUNT_LENGTH 200
#define MAX_PASSWORD_LENGTH 10
#define MAX_FILENAME_LENGTH 20
#define MAX_NAME_LENGTH 50

// Структура Material
typedef struct {
    int id;
    char name[100];
    char param[100];
    int quantity;
} Material;

// Структура Warehouse
typedef struct {
    Material* materials[1000];
    int quantity;
} Warehouse;

// Структура Product
typedef struct {
    int id;
    char customer[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    int value;
    int amount;
    int sum;
    char pay_type[10];
    char date[12];
} Product;

// Структура Order
typedef struct {
    Product product[10000];
    int quantity;
    char status[20];
} Order;

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

// Структура
typedef struct {
    Staff* staff[1000];
    int quantity;
} StaffHeadquarters;
