#define MAX_NAME_LENGTH 50

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
