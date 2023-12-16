#define MAX_DESCRIPTION_LENGTH 100
#define MAX_AMOUNT_LENGTH 200
#define MAX_PASSWORD_LENGTH 10
#define MAX_FILENAME_LENGTH 20
#define MAX_NAME_LENGTH 50

// Ваш заголовочный файл
#define COLUMN_ID 0
#define COLUMN_NAME 1
#define COLUMN_STATUS 2
#define COLUMN_POSITION 3
#define COLUMN_SALARY 4


// Структура Material
typedef struct {
    char name[100];
    char param[100];
    int quantity;
} Material;

// Структура Warehouse
typedef struct {
    Material* materials[100];
    int quantity;
} Warehouse;

// Структура Product
typedef struct {
    char customer[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    int value;
    int amount;
    char pay_type[10];
} Product;


// Структура Order
typedef struct {
    Product product[100];
    int quantity;
    char status[20];
} Order;


// Структура Staff
typedef struct {
    int id;
    char name[100];
    char status[100];
    char emp_date[12];
    char inn[11];
    char passport[10];
    char phone_num[12];
    char position[20];
    int salary;
} Staff;

// Структура 
typedef struct {
    Staff* staff[300];
    int quantity;
} StaffHeadquarters;

