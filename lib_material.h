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
