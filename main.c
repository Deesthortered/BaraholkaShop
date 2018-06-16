#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#define STR_LEN 51
#define DATE_LEN 3 // Don't change!
typedef unsigned int ull;
typedef short boolean;

ull lastID[3];
struct tm *m_time;

enum Size { NullSize, Small, Medium, Big};
enum Type { user_type, order_type, goods_type, assortment_type};
struct Admin
{
    char password[STR_LEN];
    char name[STR_LEN];
    char surname[STR_LEN];
    char father[STR_LEN];
    char phone[STR_LEN];
    char email[STR_LEN];
} admin;

// DataBase Structures
struct User
{
    char login[STR_LEN];
    char password[STR_LEN];
    char name[STR_LEN];
    char surname[STR_LEN];
    char father[STR_LEN];
    int date_birth[DATE_LEN];
    int date_reg[DATE_LEN];
    char phone[STR_LEN];
    char city[STR_LEN];
    ull count_of_orders;
    boolean blacklist;
    ull money;

    struct User * next;
    boolean mark_for_delete;
};
struct Order
{
    ull ID;
    int date[DATE_LEN];
    ull price;
    char login[STR_LEN];

    struct Order * next;
    boolean mark_for_delete;
};
struct Goods
{
    ull ID;
    char name[STR_LEN];
    ull price;
    enum Size size;
    char country[STR_LEN];
    char producer[STR_LEN];
    ull count_goods;

    ull ID_order;

    struct Goods * next;
    boolean mark_for_delete;
};
struct Assortment
{
    ull ID;
    char name[STR_LEN];
    ull price;
    enum Size size;
    boolean exist;
    char country[STR_LEN];
    char producer[STR_LEN];

    struct Assortment * next;
    boolean mark_for_delete;
};

struct Goods *basket = NULL;
struct User *current_user = NULL;

// Service functions
struct User Initializing_User(char *login, char *password, char *name, char *surname, char *father, int *date_birth, int *date_reg, char *phone, char *city, ull c, boolean bl_l, struct User *next, boolean MFD);
struct Order Initializing_Order(ull id, int *date, ull price, char *login, struct Order *next, boolean MFD);
struct Goods Initializing_Goods(ull id, char *name, ull price, enum Size size, char *country, char *producer, ull id_order, struct Goods *next, boolean MFD, ull cnt_goods);
struct Assortment Initializing_Assortment(ull id, char *name, ull price, enum Size size, boolean exist, char *country, char *producer, struct Assortment *next, boolean MFD);

ull CreateID(enum Type type);

struct User* AddUserToList(struct User *head, struct User user);
struct Order* AddOrderToList(struct Order *head, struct Order order);
struct Goods* AddGoodsToList(struct Goods *head, struct Goods goods);
struct Assortment* AddAssortmentToList(struct Assortment *head, struct Assortment assortment);

struct User* DeleteListOfUser(struct User *head);
struct Order* DeleteListOfOrder(struct Order *head);
struct Goods* DeleteListOfGoods(struct Goods *head);
struct Assortment* DeleteListOfAssortment(struct Assortment *head);

struct User* DeleteElemListOfUser(struct User *head, struct User *cur);
struct Order* DeleteElemListOfOrder(struct Order *head, struct Order *cur);
struct Goods* DeleteElemListOfGoods(struct Goods *head, struct Goods *cur);
struct Assortment* DeleteElemListOfAssortment(struct Assortment *head, struct Assortment *cur);

void AddUserToBase(struct User user);
void AddOrderToBase(struct Order order);
void AddGoodsToBase(struct Goods goods);
void AddAssortmentToBase(struct Assortment assortment);

struct User* FindUserInBase(struct User user);
struct Order* FindOrderInBase(struct Order order);
struct Goods* FindGoodsInBase(struct Goods goods);
struct Assortment* FindAssortmentInBase(struct Assortment assortment);

void EditUserBase(struct User user);
void EditOrderBase(struct Order order);
void EditGoodsBase(struct Goods goods);
void EditAssortmentBase(struct Assortment assortment);

void DeleteUserInBase(char * login);
void DeleteOrderInBase(ull id);
void DeleteGoodsInBase(ull id);
void DeleteAssortmentInBase(ull id);

void ViewAllUsers(boolean bl_l);
void ViewOrderUsers(char * log);
void ViewAssortment(void);

void ClearDataBases(void);
void DeleteDataBases(void);

// Action functions
struct Admin CreateStandartAdmin(void);
void Initialization(void);
void StartMenu(char * log);
void Autorization(char * log);
void Registration(void);

// Admin actions
void AdminInterface(void);

void AdminShowAllUsers(void);
void AdminFindUser(void);
void AdminShowBlackList(void);

boolean AdminSelectUser(void);
void AdminShowUsersOrders(char * log);
boolean AdminDeleteUser(struct User *user);

void AdminShowAssortment(void);
void AdminFindAssortment(void);
void AdminAddGoods(void);
void AdminSelectAssortment(void);
void AdminEditGoods(struct Assortment *ptr);

void AdminFindOrder(void);

void AdminManagement(void);
void AdminEditInfo(void);
void AdminEditPassword(void);

void AdminClearDataBases(void);
void AdminDeleteDataBases(void);

// User's actions
void UserInterface(char * login);

void UserViewAssortment(void);
void UserFindAssortment(void);
void UserAddGoodsToBasket(void);

void UserShowBasket(void);
void UserRemoveGoodsInBasket(void);
boolean UserCreateOrder(ull price);

void UserViewOrders(boolean adm);
void UserSelectOrder(boolean adm);

void UserMoneyManagement(void);
void UserAddMoney(void);

void UserManagementData(void);
void UserEditData(void);
void UserEditPassword(void);
void UserDeleteProfile(void);

int main(void)
{
    long int s_time;
    s_time = time(NULL);
    m_time  = localtime (&s_time);
    Initialization();
    char current_log[STR_LEN];
    StartMenu(current_log);
    if (!strcmp(current_log, "admin")) AdminInterface();
    else UserInterface(current_log);
    return 0;
}

// Action functions
struct Admin CreateStandartAdmin(void)
{
    struct Admin admin;
    strcpy(admin.password,"admin");
    strcpy(admin.name, "Anatoly");
    strcpy(admin.surname, "Davydko");
    strcpy(admin.father, "Evgenievich");
    strcpy(admin.phone, "+380500146257");
    strcpy(admin.email, "anatoly.davydko@gmail.com");
    return admin;
};
void Initialization(void)
{
    FILE *f;
    f = fopen("admin", "r+b");
    if (f == NULL)
    {
        f = fopen("admin", "wb");
        struct Admin admin = CreateStandartAdmin();
        fwrite(&admin, sizeof(struct Admin), 1, f);
    }
    else
    {
        fread(&admin, sizeof(struct Admin), 1, f);
    }
    fclose(f);

    f = fopen("user.data", "r+b");
    if (f == NULL) {f = fopen("user.data", "wb");}
    fclose(f);

    f = fopen("order.data", "r+b");
    if (f == NULL) {f = fopen("order.data", "wb"); lastID[0] = 0;}
    else
    {
        struct Order order; order.ID = 0;
        fseek(f,-sizeof(struct Order), SEEK_END);
        fread(&order, sizeof(struct Order), 1, f);
        lastID[0] = order.ID;
    }
    fclose(f);

    f = fopen("goods.data", "r+b");
    if (f == NULL) {f = fopen("goods.data", "wb"); lastID[1] = 0;}
    else
    {
        struct Goods goods; goods.ID = 0;
        fseek(f,-sizeof(struct Goods), SEEK_END);
        fread(&goods, sizeof(struct Goods), 1, f);
        lastID[1] = goods.ID;
    }
    fclose(f);

    f = fopen("assortment.data", "r+b");
    if (f == NULL) {f = fopen("assortment.data", "wb"); lastID[2] = 0;}
    else
    {
        struct Assortment assortment; assortment.ID = 0;
        fseek(f,-sizeof(struct Assortment), SEEK_END);
        fread(&assortment, sizeof(struct Assortment), 1, f);
        lastID[2] = assortment.ID;
    }
    fclose(f);
}
void StartMenu(char * log)
{
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("***********************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("*****************************||| Start Menu |||******************************\n");
            printf("1) Authorization\n");
            printf("2) Registration\n");
            printf("3) Quit\n");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; Autorization(log); if (strcmp(log,"-")) return; } break;
            case '2': { f = 1; Registration(); } break;
            case '3': { f = 1; system("cls"); exit(0);} break;
        }
    }
}
void Autorization(char * log)
{
    char pass[STR_LEN];
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("*****************************||| Authorization |||****************************\n");
    printf("\n   Login: "); scanf("%s", log);
    printf("Password: "); scanf("%s", pass);
    if (!strcmp(log, "admin"))
    {
        if(!strcmp(pass, admin.password))
        {
            printf("\nWelcome, my Lord!");
            getch();
        }
        else
        {
            printf("\nYou are not my Lord, you are bastard!!!");
            getch();
            strcpy(log, "-");
        }
    }
    else
    {
        struct User * list = NULL;
        list = FindUserInBase(Initializing_User(log, pass, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, 0));
        if (list == NULL)
        {
            printf("\nWrong data.");
            getch();
            strcpy(log, "-");
        }
        else
        {
            strcpy(log, list->login);
            printf("\nWelcome, %s %s %s", list->surname, list->name, list->father);
            getch();
            list = DeleteListOfUser(list);
        }
    }
}
void Registration(void)
{
    char log[STR_LEN];
    struct User * ch_list = NULL;
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("*****************************||| Registration |||*****************************\n");
    printf("    Login: "); scanf("%s", log);
    if (!strcmp(log, "admin"))
    {
        printf("\nYou don't have permission for this login!\n");
        getch();
        return;
    }
    ch_list = FindUserInBase(Initializing_User(log, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, 0));
    if (ch_list != NULL)
    {
        printf("\nUser with this login is already exist!\n");
        getch();
        ch_list = DeleteListOfUser(ch_list);
        return;
    }
    struct User user;
    strcpy(user.login, log);
    printf(" Password: "); scanf("%s", log); strcpy(user.password, log);
    printf("     Name: "); scanf("%s", log); strcpy(user.name, log);
    printf("  Surname: "); scanf("%s", log); strcpy(user.surname, log);
    printf("   Father: "); scanf("%s", log); strcpy(user.father, log);
    printf("Date of birth.\n Day: "); scanf("%i", &user.date_birth[0]); printf(" Month: "); scanf("%i", &user.date_birth[1]); printf(" Year: "); scanf("%i", &user.date_birth[2]);
    printf("    Phone: "); scanf("%s", log); strcpy(user.phone, log);
    printf("     City: "); scanf("%s", log); strcpy(user.city, log);
    user.count_of_orders = 0;
    user.blacklist = 0;
    user.next = NULL;
    user.mark_for_delete = 0;
    user.money = 0;

    user.date_reg[0] = m_time->tm_mday;
    user.date_reg[1] = m_time->tm_mon+1;
    user.date_reg[2] = m_time->tm_year + 1900;

    AddUserToBase(user);
    printf("\nNew user has added to base succesfully!");
    getch();
}

// Admin actions
void AdminInterface(void)
{
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| Admin Interface |||***************************\n");
            printf("Admin: %s %s\n", admin.surname, admin.name);
            printf("\n* Management of users:");
            printf("\n1) Show all users.");
            printf("\n2) Find users.");
            printf("\n3) Show blacklist.");
            printf("\n\n* Management of assortment:");
            printf("\n4) Show assortment.");
            printf("\n5) Find goods.");
            printf("\n6) Add goods.");
            printf("\n\n* Monitoring orders");
            printf("\n7) Show last orders.");
            printf("\n8) Find order.\n");
            printf("\nA) Management of admin data.");
            printf("\n\n* Management of databases.");
            printf("\nC) Clear databases.");
            printf("\nR) Delete databases.");
            printf("\n\n\n0) Exit.");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; AdminShowAllUsers(); } break;
            case '2': { f = 1; AdminFindUser(); } break;
            case '3': { f = 1; AdminShowBlackList(); } break;
            case '4': { f = 1; AdminShowAssortment(); } break;
            case '5': { f = 1; AdminFindAssortment(); } break;
            case '6': { f = 1; AdminAddGoods(); } break;
            case '7': { f = 1; UserViewOrders(1); } break;
            case '8': { f = 1; AdminFindOrder(); } break;
            case 'A': { f = 1; AdminManagement(); } break;
            case 'C': { f = 1; AdminClearDataBases(); } break;
            case 'R': { f = 1; AdminDeleteDataBases(); } break;
            case '0': { system("cls"); exit(0); } break;
        }
    }
}

void AdminShowAllUsers(void)
{
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| Admin Interface |||***************************\n");
            printf("Admin: %s %s\n\n", admin.surname, admin.name);
            printf("List of users:\n");
            ViewAllUsers(0);
            printf("\n");
            printf("1) Select user (login).\n");
            printf("0) Back.\n");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; AdminSelectUser(); } break;
            case '0': { return; } break;
        }
    }
}
void AdminFindUser(void)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| Admin Interface |||***************************\n");
    printf("Admin: %s %s\n\n", admin.surname, admin.name);
    printf("Find user:\n");
    struct User user = Initializing_User(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, 0);
    struct User *list = NULL;
    char log[STR_LEN]; int i = 0; ull ulltmp = 0;
    printf("           Login: "); scanf("%s", log);
    if (strcmp(log, "0"))
    {
        strcpy(user.login, log);
    }
    else
    {
        printf("            Name: "); scanf("%s", log); if (strcmp(log, "0")) strcpy(user.name, log);
        printf("         Surname: "); scanf("%s", log); if (strcmp(log, "0")) strcpy(user.surname, log);
        printf("          Father: "); scanf("%s", log); if (strcmp(log, "0")) strcpy(user.father, log);
        printf("            City: "); scanf("%s", log); if (strcmp(log, "0")) strcpy(user.city, log);
        printf("           Phone: "); scanf("%s", log); if (strcmp(log, "0")) strcpy(user.phone, log);
        printf(" Count of orders: ");  scanf("%u", &ulltmp); if (ulltmp != 0) user.count_of_orders = ulltmp;
        printf("   Date of birth:\n");
        printf("       Day: "); scanf("%i", &i); if (i != 0) user.date_birth[0] = i;
        printf("     Month: "); scanf("%i", &i); if (i != 0) user.date_birth[1] = i;
        printf("      Year: "); scanf("%i", &i); if (i != 0) user.date_birth[2] = i;
        printf("    Date of reg.:\n");
        printf("       Day: "); scanf("%i", &i); if (i != 0) user.date_reg[0] = i;
        printf("     Month: "); scanf("%i", &i); if (i != 0) user.date_reg[1] = i;
        printf("      Year: "); scanf("%i", &i); if (i != 0) user.date_reg[2] = i;
    }
    list = FindUserInBase(user);
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| Admin Interface |||***************************\n");
            printf("Admin: %s %s\n\n", admin.surname, admin.name);
            printf("List of users:\n");
            struct User *view = list; int i = 1;
            while (view)
            {
                printf("%i)  Login:  %s\n",i , view->login);
                printf("     Name:  %s %s\n", view->surname, view->name);
                printf("     City:  %s\n", view->city);
                printf("    Phone:  %s\n\n", view->phone);
                i++;
                view = view->next;
            }
            printf("\n\n");
            printf("1) Select user (login).\n");
            printf("0) Back.\n");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; if (AdminSelectUser()) { list = DeleteListOfUser(list); } } break;
            case '0': { return; } break;
        }
    }
}
void AdminShowBlackList(void)
{
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| Admin Interface |||***************************\n");
            printf("Admin: %s %s\n\n", admin.surname, admin.name);
            printf("List of users:\n");
            ViewAllUsers(1);
            printf("\n");
            printf("1) Select user (login).\n");
            printf("0) Back.\n");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; AdminSelectUser(); } break;
            case '0': { return; } break;
        }
    }
}
boolean AdminSelectUser(void)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| Admin Interface |||***************************\n");
    printf("Admin: %s %s\n\n", admin.surname, admin.name);
    printf("Select user:\n");
    char log[STR_LEN];
    printf(" Login: "); scanf("%s", log);
    struct User *user = FindUserInBase(Initializing_User(log, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, 0));
    if (user == NULL)
    {
        printf("\n\nWrong data!\n");
        getch();
        return 0;
    }
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| Admin Interface |||***************************\n");
            printf("Admin: %s %s\n\n", admin.surname, admin.name);
            printf("User:\n\n");
            printf("           Login: %s\n", user->login);
            printf("            Name: %s\n", user->name);
            printf("         Surname: %s\n", user->surname);
            printf("          Father: %s\n", user->father);
            printf("            City: %s\n", user->city);
            printf("           Phone: %s\n", user->phone);
            printf("   Date of birth: %i.%i.%i\n", user->date_birth[0], user->date_birth[1], user->date_birth[2]);
            printf("    Date of reg.: %i.%i.%i\n", user->date_reg[0], user->date_reg[1], user->date_reg[2]);
            printf("           Money: %u\n", user->money);
            printf(" Count of orders: %u\n", user->count_of_orders);
            printf("       BlackList: %s\n", (user->blacklist) ? "+" : "-" );

            printf("\n\n");
            printf("1) Show user's orders.\n");
            printf("2) %s.\n", (user->blacklist) ? "Out from Black List" : "Into Black List");
            printf("3) Delete.\n");
            printf("0) Back.\n");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; AdminShowUsersOrders(user->login); } break;
            case '2': { f = 1; user->blacklist = !(user->blacklist); EditUserBase(*user);} break;
            case '3': { f = 1; if (AdminDeleteUser(user)) {user = DeleteListOfUser(user); return 1;} } break;
            case '0': { user = DeleteListOfUser(user); return 0; } break;
        }
    }
}
void AdminShowUsersOrders(char * log)
{
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| Admin Interface |||***************************\n");
            printf("Admin: %s %s\n\n", admin.surname, admin.name);
            printf("User's orders:\n");
            ViewOrderUsers(log);
            printf("\n\n");
            printf("1) Select order.\n");
            printf("2) Back.\n");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1' : { f = 1; UserSelectOrder(1); } break;
            case '0' : { return; } break;
        }
    }
}
boolean AdminDeleteUser(struct User *user)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| Admin Interface |||***************************\n");
    printf("Admin: %s %s\n\n", admin.surname, admin.name);
    printf("Are you really want to delete %s %s (%s)? (Y/N)\n", user->name, user->surname, user->login);
    char c;
    while (1)
    {
        c = getch();
        if (c == 'y' || c == 'Y')
            {
                DeleteUserInBase(user->login);
                return 1;
            }
        if (c == 'n' || c == 'N') { return 0;}
    }
}

void AdminShowAssortment()
{
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| Admin Interface |||***************************\n");
            printf("Admin: %s %s\n\n", admin.surname, admin.name);
            printf("Assortment:\n");
            ViewAssortment();
            printf("\n\n1) Select goods (by ID).");
            printf("\n0) Back");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; AdminSelectAssortment(); return; } break;
            case '0': { return;} break;
        }
    }
}
void AdminFindAssortment(void)
{
    struct Assortment * list = NULL;
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| Admin Interface |||***************************\n");
    printf("Admin: %s %s\n\n", admin.surname, admin.name);
    printf("Find Goods: (write fields to search, other - write 0)\n\n");
    struct Assortment temp = Initializing_Assortment(0, NULL, 0, NullSize, 0, NULL, NULL, NULL, 0);
    ull ulltemp = 0; char strtmp[STR_LEN];
    printf("       ID: "); scanf("%u", &ulltemp);
    if (ulltemp != 0)
    {
        temp.ID = ulltemp;
    }
    else
    {
        printf("     Name: "); scanf("%s", strtmp); if (strcmp(strtmp, "0")) strcpy(temp.name, strtmp);
        printf("    Sizes: 1 - Small, 2 - Medium, 3 - Big.\n");
        printf("     Size: "); int i = 0; scanf("%i", &i); temp.size = i;
        printf("  Country: "); scanf("%s", strtmp); if (strcmp(strtmp, "0")) strcpy(temp.country, strtmp);
        printf(" Producer: "); scanf("%s", strtmp); if (strcmp(strtmp, "0")) strcpy(temp.producer, strtmp);
    }
    list = FindAssortmentInBase(temp);
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| Admin Interface |||***************************\n");
            printf("Admin: %s %s\n\n", admin.surname, admin.name);

            struct Assortment * view = list; int i = 1;
            while (view)
            {
                printf("%i) ID: %u\n", i, view->ID);
                printf("    Name: %s\n", view->name);
                printf("    Producer: %s\n", view->producer);
                printf("    Country: %s\n", view->country);
                printf("    Price: %u\n", view->price);
                printf("    Exist: %s\n\n", (view->exist) ? "+" : "-");
                i++;
                view = view->next;
            }

            printf("\n\n1) Select goods (by ID).");
            printf("\n0) Back");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; list = DeleteListOfAssortment(list); AdminSelectAssortment(); return;} break;
            case '0': { list = DeleteListOfAssortment(list); return;} break;
        }
    }
}
void AdminAddGoods(void)
{
    struct Assortment temp;
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| Admin Interface |||***************************\n");
    printf("Admin: %s %s\n\n", admin.surname, admin.name);
    printf("Add goods to assortment.\n");
    temp.ID = CreateID(assortment_type);
    printf("\n     ID: %u", temp.ID);
    printf("\n     Name: "); scanf("%s", temp.name);
    printf("    Price: "); scanf("%u", &temp.price);
    printf("    Sizes: 1 - Small, 2 - Medium, 3 - Big.\n");
    printf("     Size: "); int i; scanf("%i", &i); temp.size = i;
    printf("  Country: "); scanf("%s", temp.country);
    printf(" Producer: "); scanf("%s", temp.producer);
    temp.exist = 1;
    temp.next = NULL;
    temp.mark_for_delete = 0;
    AddAssortmentToBase(temp);
    printf("\nDone.\n");
    getch();
}
void AdminSelectAssortment(void)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| Admin Interface |||***************************\n");
    printf("Admin: %s %s\n\n", admin.surname, admin.name);
    ull id = 0;
    printf(" Enter ID: "); scanf("%u", &id);
    struct Assortment* list = FindAssortmentInBase(Initializing_Assortment(id, NULL, 0, NullSize, 0, NULL, NULL, NULL, 0));

    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| Admin Interface |||***************************\n");
            printf("Admin: %s %s\n\n", admin.surname, admin.name);
            if (list == NULL)
            {
                printf("Not found.\n");
                getch();
                return;
            }
            printf("       ID: %u\n", list->ID);
            printf("     Name: %s\n", list->name);
            printf(" Producer: %s\n", list->producer);
            printf("  Country: %s\n", list->country);
            printf("    Price: %u\n", list->price);
            printf("    Exist: %s\n\n\n", (list->exist) ? "+" : "-");
            printf("1) Edit goods.\n");
            printf("2) Delete goods.\n");
            printf("3) %s\n\n", (list->exist) ? "Is not exist" : "Is exist");
            printf("0) Back\n");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; AdminEditGoods(list); } break;
            case '2': { f = 1; DeleteAssortmentInBase(list->ID); list = DeleteListOfAssortment(list); return; } break;
            case '3': { f = 1; list->exist = !(list->exist); EditAssortmentBase(*list); } break;
            case '0': { list = DeleteListOfAssortment(list); return; } break;
        }
    }
}
void AdminEditGoods(struct Assortment *ptr)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| Admin Interface |||***************************\n");
    printf("Admin: %s %s\n\n", admin.surname, admin.name);
    char strtmp[STR_LEN]; ull price = 0;
    printf("Edit Goods: (write fields to edit, other - write 0)\n\n");
    printf("     Name: "); scanf("%s", strtmp); if (strcmp(strtmp, "0")) strcpy(ptr->name, strtmp);
    printf("    Price: "); scanf("%u", &price); if (price != 0) ptr->price = price;
    printf("    Sizes: 1 - Small, 2 - Medium, 3 - Big.\n");
    printf("     Size: "); int i = 0; scanf("%i", &i); ptr->size = i;
    printf("  Country: "); scanf("%s", strtmp); if (strcmp(strtmp, "0")) strcpy(ptr->country, strtmp);
    printf(" Producer: "); scanf("%s", strtmp); if (strcmp(strtmp, "0")) strcpy(ptr->producer, strtmp);
    EditAssortmentBase(*ptr);
    printf("\nDone.\n");
    getch();
}

void AdminFindOrder(void)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| Admin Interface |||***************************\n");
    printf("Admin: %s %s\n\n", admin.surname, admin.name);
    printf("Find order: (write fields to edit, other - write 0)\n\n");
    struct Order order = Initializing_Order(0, NULL, 0, NULL, NULL, 0);
    char strtmp[STR_LEN]; ull ulltemp = 0; int i = 0;
    printf("       ID: "); scanf("%u", &ulltemp);
    if (ulltemp != 0)
    {
        order.ID = ulltemp;
    }
    else
    {
        printf("    Login: "); scanf("%s", strtmp); if(strcmp(strtmp, "0")) strcpy(order.login, strtmp);
        printf("    Price: "); scanf("%u", &ulltemp); if (ulltemp != 0) order.price = ulltemp;
        printf("     Date: \n");
        printf("   Day: "); scanf("%i", &i); if (i != 0) order.date[0] = i;
        printf(" Month: "); scanf("%i", &i); if (i != 0) order.date[1] = i;
        printf("  Year: "); scanf("%i", &i); if (i != 0) order.date[2] = i;
    }
    struct Order *list = FindOrderInBase(order);
    if (list == NULL)
    {
        printf("\n\nWrong data!\n");
    }
    else
    {
        char c; boolean f = 1;
        while (1)
        {
            if (f)
            {
                system("cls");
                printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
                printf("****************************||| Admin Interface |||***************************\n");
                printf("Admin: %s %s\n\n", admin.surname, admin.name);
                printf("Orders: \n\n");
                struct Order *view = list; i = 1;
                while (view)
                {
                    printf("%i)   ID order: %u\n", i, view->ID);
                    printf("         User: %s\n", view->login);
                    printf("        Price: %u\n", view->price);
                    printf("         Date: %i.%i.%i\n", view->date[0], view->date[1], view->date[2]);
                    printf("        Goods: \n");
                    struct Goods *goods = FindGoodsInBase(Initializing_Goods(0, NULL, 0, NullSize, NULL, NULL, view->ID, NULL, 0, 0));
                    struct Goods *view_goods = goods;
                    while (view_goods)
                    {
                        printf("              *%s\n", view_goods->name);
                        view_goods = view_goods->next;
                    }
                    goods = DeleteListOfGoods(goods);
                    printf("\n\n\n");
                    view=view->next;
                }
                printf("1) Select order (ID).\n");
                printf("0) Back.\n");
                f = 0;
            }
            c = getch();
            switch (c)
            {
                case '1': { f = 1; UserSelectOrder(1); } break;
                case '0': { list = DeleteListOfOrder(list); return; } break;
            }
        }
    }
    getch();
}

void AdminManagement(void)
{
    char c; boolean f = 1; boolean edit = 0;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| Admin Interface |||***************************\n");
            printf("Admin: %s %s\n\n", admin.surname, admin.name);
            printf("Admin information:\n\n");
            printf("     Name: %s\n", admin.name);
            printf("  Surname: %s\n", admin.surname);
            printf("   Father: %s\n", admin.father);
            printf("    Phone: %s\n", admin.phone);
            printf("   E-mail: %s\n", admin.email);
            printf("\n\n1) Edit admin information.");
            printf("\n2) Edit password.");
            printf("\n\n0) Back.");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; edit = 1; AdminEditInfo();} break;
            case '2': { f = 1; edit = 1; AdminEditPassword(); } break;
            case '0':
                {
                    if (edit)
                    {
                        FILE * f = fopen("admin", "wb");
                        fwrite(&admin, sizeof(struct Admin), 1, f);
                        fclose(f);
                    }
                    return;
                } break;
        }
    }
}
void AdminEditInfo(void)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| Admin Interface |||***************************\n");
    char temp[STR_LEN];
    printf("\nEdit admin information. (if you don't want edit - write 0)");
    printf("\n     Name: "); scanf("%s", temp); if (strcmp(temp, "0")) strcpy(admin.name, temp);
    printf("  Surname: "); scanf("%s", temp); if (strcmp(temp, "0")) strcpy(admin.surname, temp);
    printf("   Father: "); scanf("%s", temp); if (strcmp(temp, "0")) strcpy(admin.father, temp);
    printf("    Phone: "); scanf("%s", temp); if (strcmp(temp, "0")) strcpy(admin.phone, temp);
    printf("   E-mail: "); scanf("%s", temp); if (strcmp(temp, "0")) strcpy(admin.email, temp);
    printf("\nDone.  ");
    getch();
}
void AdminEditPassword(void)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| Admin Interface |||***************************\n");
    char temp[STR_LEN];
    strcpy(temp, admin.password);
    printf("\nEnter old password: "); scanf("%s", temp);
    if (strcmp(admin.password, temp))
    {
        printf("Wrong password!\n");
        getch();
        return;
    }
    printf("Enter new password: "); scanf("%s", temp);
    strcpy(admin.password, temp);
    printf("\nDone.\n");
    getch();
}

void AdminClearDataBases(void)
{
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| Admin Interface |||***************************\n");
            printf("Admin: %s %s\n", admin.surname, admin.name);
            printf("\nDo you really want to clear all the databases? (Y/N)\n");
            f = 0;
        }
        c = getch();
        if (c == 'y' || c == 'Y') { ClearDataBases(); return;}
        if (c == 'n' || c == 'N') return;
    }
}
void AdminDeleteDataBases(void)
{
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| Admin Interface |||***************************\n");
            printf("Admin: %s %s\n", admin.surname, admin.name);
            printf("\nDo you really want to delete all the databases? \nIt will not be possible to change. (Y/N)\n");
            f = 0;
        }
        c = getch();
        if (c == 'y' || c == 'Y') { DeleteDataBases(); return;}
        if (c == 'n' || c == 'N') return;
    }
}

// User's actions
void UserInterface(char * login)
{
    current_user = FindUserInBase(Initializing_User(login, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, 0));
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| User Interface |||****************************\n");
            printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
            printf("1) Show all assortment.\n");
            printf("2) Find goods.\n");
            printf("3) Show Basket.\n\n");
            printf("4) Show old orders.\n");
            printf("5) Money.\n");
            printf("6) Management of user data.\n");
            printf("\n0) Exit.");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1':{ f = 1; UserViewAssortment(); } break;
            case '2':{ f = 1; UserFindAssortment(); } break;
            case '3':{ f = 1; UserShowBasket(); } break;
            case '4':{ f = 1; UserViewOrders(0); } break;
            case '5':{ f = 1; UserMoneyManagement(); } break;
            case '6':{ f = 1; UserManagementData(); } break;
            case '0': { system("cls"); current_user = DeleteListOfUser(current_user); basket = DeleteListOfGoods(basket); exit(0); } break;
        }
    }
}

void UserViewAssortment(void)
{
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| User Interface |||****************************\n");
            printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
            printf("Assortment:\n");
            ViewAssortment();
            printf("\n\n");
            printf("1) Add goods to basket (ID, count).\n");
            printf("0) Back.\n");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; UserAddGoodsToBasket(); } break;
            case '0': { return; } break;
        }
    }
}
void UserFindAssortment(void)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| User Interface |||****************************\n");
    printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
    printf("Find assortment: (if you don't want edit - write 0)\n");
    struct Assortment * list = NULL;
    struct Assortment temp = Initializing_Assortment(0, NULL, 0, NullSize, 0, NULL, NULL, NULL, 0);
    ull ulltemp = 0; char strtmp[STR_LEN];
    printf("       ID: "); scanf("%u", &ulltemp);
    if (ulltemp != 0)
    {
        temp.ID = ulltemp;
    }
    else
    {
        printf("     Name: "); scanf("%s", strtmp); if (strcmp(strtmp, "0")) strcpy(temp.name, strtmp);
        printf("    Sizes: 1 - Small, 2 - Medium, 3 - Big.\n");
        printf("     Size: "); int i = 0; scanf("%i", &i); temp.size = i;
        printf("  Country: "); scanf("%s", strtmp); if (strcmp(strtmp, "0")) strcpy(temp.country, strtmp);
        printf(" Producer: "); scanf("%s", strtmp); if (strcmp(strtmp, "0")) strcpy(temp.producer, strtmp);
    }
    list = FindAssortmentInBase(temp);
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| User Interface |||****************************\n");
            printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
            struct Assortment * view = list; int i = 1;
            while (view)
            {
                printf("%i) ID: %u\n", i, view->ID);
                printf("    Name: %s\n", view->name);
                printf("    Producer: %s\n", view->producer);
                printf("    Country: %s\n", view->country);
                printf("    Price: %u\n", view->price);
                printf("    Exist: %s\n\n", (view->exist) ? "+" : "-");
                i++;
                view = view->next;
            }
            printf("\n\n1) Add goods to basket (ID, count).");
            printf("\n0) Back");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; UserAddGoodsToBasket(); list = DeleteListOfAssortment(list); return;} break;
            case '0': { list = DeleteListOfAssortment(list); return;} break;
        }
    }

}
void UserAddGoodsToBasket(void)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| User Interface |||****************************\n");
    printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
    printf("Add goods to basket:\n\n");
    int id = 0, cnt = 0;
    printf("    ID: "); scanf("%i", &id);
    printf(" Count: "); scanf("%i", &cnt);
    if (id <= 0 || cnt <= 0)
    {
        printf("\nWrong data!");
        getch();
        return;
    }
    struct Assortment *temp = FindAssortmentInBase(Initializing_Assortment(id, NULL, 0, NullSize, 0, NULL, NULL, NULL, 0));
    if (temp == NULL)
    {
        printf("\nWrong data!");
        getch();
        return;
    }
    struct Goods *ptr = basket;
    while (ptr != NULL)
    {
        if (ptr->ID == temp->ID)
        {
            ptr->count_goods += cnt;
            temp = DeleteListOfAssortment(temp);
            printf("\nDone.\n");
            getch();
            return;
        }
        ptr = ptr->next;
    }
    basket = AddGoodsToList(basket, Initializing_Goods(id, temp->name, temp->price, temp->size, temp->country, temp->producer, 0, NULL, 0, cnt));
    temp = DeleteListOfAssortment(temp);
    printf("\nDone.\n");
    getch();
    return;
}

void UserShowBasket(void)
{
    char c; boolean f = 1; ull price = 0;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| User Interface |||****************************\n");
            printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
            printf("Basket:\n\n");
            struct Goods * view = basket; int i = 1;
            while (view != NULL)
            {
                printf("%i)   ID: %u\n", i, view->ID);
                printf("     Name: %s\n", view->name);
                printf(" Producer: %s\n", view->producer);
                printf("  Country: %s\n", view->country);
                printf("    Price: %u\n", view->price);
                printf("    Count: %u\n\n", view->count_goods);
                i++;
                price += view->price * view->count_goods;
                view = view->next;
            }
            printf("     Price: %u\n", price);
            printf("Your money: %u\n\n", current_user->money);
            printf("1) Create order.\n");
            printf("2) Remove goods (ID).\n");
            printf("3) Clear basket.\n");
            printf("0) Back.\n");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; if (basket) if (UserCreateOrder(price)) { basket = DeleteListOfGoods(basket); price = 0; return; }; } break;
            case '2': { f = 1; UserRemoveGoodsInBasket(); price = 0; } break;
            case '3': { f = 1; basket = DeleteListOfGoods(basket); price = 0; } break;
            case '0': { return; } break;
        }
    }
}
void UserRemoveGoodsInBasket(void)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| User Interface |||****************************\n");
    printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
    printf("Remove goods in basket:\n\n");
    ull id = 0;
    printf("  ID: "); scanf("%u", &id);
    struct Goods *tmp = basket;
    while (tmp != NULL)
    {
        if (tmp->ID == id) break;
        tmp = tmp->next;
    }
    if (tmp == NULL)
    {
        printf("\nNot found!");
    }
    else
    {
        basket = DeleteElemListOfGoods(basket, tmp);
        printf("\nDone.\n");
    }
    getch();
}
boolean UserCreateOrder(ull price)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| User Interface |||****************************\n");
    printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
    printf("Create order:\n\n");
    if (current_user->money < price)
    {
        printf("Sorry, but you don't have enough money.\n");
        getch();
        return 0;
    }
    struct Order order;
    order.ID = CreateID(order_type);
    order.date[0] = m_time->tm_mday;
    order.date[1] = m_time->tm_mon+1;
    order.date[2] = m_time->tm_year + 1900;
    strcpy(order.login, current_user->login);
    order.price = price;
    order.mark_for_delete = 0;
    order.next = NULL;
    printf("      ID: %u\n", order.ID);
    printf("    User: %s\n", order.login);
    printf("    Date: %i.%i.%i\n", order.date[0], order.date[1], order.date[2]);
    printf("   Goods:\n\n");
    struct Goods *tmp = basket; int i = 1;
    while (tmp != NULL)
    {
        printf("%i)   ID: %u\n", i, tmp->ID);
        printf("     Name: %s\n", tmp->name);
        printf(" Producer: %s\n", tmp->producer);
        printf("  Country: %s\n", tmp->country);
        printf("    Price: %u\n", tmp->price);
        printf("    Count: %u\n\n", tmp->count_goods);
        i++;
        tmp->ID_order = order.ID;
        tmp = tmp->next;
    }
    printf("\n\n      Price: %u\n", order.price);
    printf(" Your money: %u\n\n", current_user->money);
    printf("Are you sure? (Y/N)\n");
    char c;
    while (1)
    {
        c = getch();
        if (c == 'y' || c == 'Y')
        {
            tmp = basket;
            while (tmp != NULL)
            {
                AddGoodsToBase(*tmp);
                tmp = tmp->next;
            }
            AddOrderToBase(order);
            current_user->money -= price;
            current_user->count_of_orders++;
            EditUserBase(*current_user);
            printf("\nDone.\n");
            getch();
            return 1;
        }
        if (c == 'n' || c == 'N')
        {
            lastID[0]--;
            return 0;
        }
    }
}

void UserViewOrders(boolean adm)
{
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            if (adm)
            {
                printf("****************************||| Admin Interface |||***************************\n");
                printf("Admin: %s %s\n\n", admin.surname, admin.name);
            }
            else
            {
                printf("****************************||| User Interface |||****************************\n");
                printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
            }
            printf("Orders:\n\n");
            if (adm) ViewOrderUsers("admin"); else ViewOrderUsers(current_user->login);
            printf("1) Select order (ID).\n");
            printf("0) Back.\n");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; UserSelectOrder(adm); } break;
            case '0': { return; } break;
        }
    }
}
void UserSelectOrder(boolean adm)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    if (adm)
    {
        printf("****************************||| Admin Interface |||***************************\n");
        printf("Admin: %s %s\n\n", admin.surname, admin.name);
    }
    else
    {
        printf("****************************||| User Interface |||****************************\n");
        printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
    }
    printf("Select order:\n\n");
    ull temp = 0;
    printf("   ID: "); scanf("%u", &temp);
    struct Order *order = FindOrderInBase(Initializing_Order(temp, NULL, 0, (adm) ? (NULL) : (current_user->login), NULL, 0));
    if (order == NULL)
    {
        printf("\nWrong data!\n");
        getch();
        return;
    }
    struct Goods *goods = FindGoodsInBase(Initializing_Goods(0, NULL, 0, NullSize, NULL, NULL, order->ID, NULL, 0, 0));
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    if (adm)
    {
        printf("****************************||| Admin Interface |||***************************\n");
        printf("Admin: %s %s\n\n", admin.surname, admin.name);
    }
    else
    {
        printf("****************************||| User Interface |||****************************\n");
        printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
    }
    printf("Order:\n\n");
    printf("       ID order: %u\n", order->ID);
    printf("           User: %s\n", order->login);
    printf("           Date: %i.%i.%i\n", order->date[0], order->date[1], order->date[2]);
    printf("          Price: %u\n", order->price);
    printf(" Goods:\n\n");
    struct Goods *ptr = goods; int i = 1;
    while (ptr != NULL)
    {
        printf("%i)   Name: %s\n", i, ptr->name);
        printf("       ID: %u\n", ptr->ID);
        printf("    Count: %u\n", ptr->count_goods);
        printf("    Price: %u\n", ptr->price);
        printf("  Country: %s\n", ptr->country);
        printf(" Producer: %s\n\n", ptr->producer);
        i++;
        ptr = ptr->next;
    }
    printf("\n0) Back.\n");
    char c;
    while (1)
    {
        c = getch();
        if (c == '0')
        {
            order = DeleteListOfOrder(order);
            goods = DeleteListOfGoods(goods);
            return;
        }
    }
}

void UserMoneyManagement(void)
{
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| User Interface |||****************************\n");
            printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
            printf("User money: %u\n\n", current_user->money);
            printf("1) Add money\n");
            printf("0) Back\n");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; UserAddMoney(); } break;
            case '0': { return; } break;
        }
    }
}
void UserAddMoney(void)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| User Interface |||****************************\n");
    printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
    int i = 0;
    printf("Enter summ: "); scanf("%i", &i);
    if (i <= 0)
    {
        printf("Wrong data!");
    }
    else
    {
        current_user->money += i;
        EditUserBase(*current_user);
        printf("\nDone.\n");
    }
    getch();
}

void UserManagementData(void)
{
    char c; boolean f = 1;
    while (1)
    {
        if (f)
        {
            system("cls");
            printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
            printf("****************************||| User Interface |||****************************\n");
            printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
            printf("User data:\n");
            printf("      Login: %s\n", current_user->login);
            printf("       Name: %s\n", current_user->name);
            printf("    Surname: %s\n", current_user->surname);
            printf("     Father: %s\n", current_user->father);
            printf("       City: %s\n", current_user->city);
            printf("      Phone: %s\n", current_user->phone);
            printf(" Date of birth:\n");
            printf("    Day: %i\n", current_user->date_birth[0]);
            printf("  Month: %i\n", current_user->date_birth[1]);
            printf("   Year: %i\n", current_user->date_birth[2]);
            printf(" Date of registration:\n");
            printf("    Day: %i\n", current_user->date_reg[0]);
            printf("  Month: %i\n", current_user->date_reg[1]);
            printf("   Year: %i\n", current_user->date_reg[2]);
            printf("     Orders: %u\n\n", current_user->count_of_orders);
            printf("1) Edit information.\n");
            printf("2) Edit password.\n");
            printf("3) Delete profile.\n");
            printf("0) Back.");
            f = 0;
        }
        c = getch();
        switch (c)
        {
            case '1': { f = 1; UserEditData(); } break;
            case '2': { f = 1; UserEditPassword(); } break;
            case '3': { f = 1; UserDeleteProfile(); } break;
            case '0': { return; } break;
        }
    }
}
void UserEditData(void)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| User Interface |||****************************\n");
    printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
    printf("Edit user data. (if you don't want edit - write 0)\n");
    char temp[STR_LEN];
    printf("     Name: "); scanf("%s", temp); if (strcmp(temp, "0")) strcpy(current_user->name, temp);
    printf("  Surname: "); scanf("%s", temp); if (strcmp(temp, "0")) strcpy(current_user->surname, temp);
    printf("   Father: "); scanf("%s", temp); if (strcmp(temp, "0")) strcpy(current_user->father, temp);
    printf("    Phone: "); scanf("%s", temp); if (strcmp(temp, "0")) strcpy(current_user->phone, temp);
    printf("     City: "); scanf("%s", temp); if (strcmp(temp, "0")) strcpy(current_user->city, temp);
    printf("Date of birth.\n"); int n = 0;
    printf("   Day: "); scanf("%i", &n); if (n != 0) current_user->date_birth[0] = n;
    printf(" Month: "); scanf("%i", &n); if (n != 0) current_user->date_birth[1] = n;
    printf("  Year: "); scanf("%i", &n); if (n != 0) current_user->date_birth[2] = n;
    EditUserBase(*current_user);
    printf("\nDone.\n");
    getch();
}
void UserEditPassword(void)
{
    system("cls");
    printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
    printf("****************************||| User Interface |||****************************\n");
    printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
    char pass[STR_LEN];
    printf("Old password: "); scanf("%s", pass);
    if (strcmp(pass, current_user->password))
    {
        printf("\nWrong password.");
        getch();
        return;
    }
    printf("New password: "); scanf("%s", pass);
    strcpy(current_user->password, pass);
    EditUserBase(*current_user);
    printf("\nDone.\n");
    getch();
}
void UserDeleteProfile(void)
{
    char c;
    while (1)
    {
        system("cls");
        printf("************************||| Internet-Shop BARAHOLKA |||***********************\n");
        printf("****************************||| User Interface |||****************************\n");
        printf("Current profile: %s %s\n\n", current_user->surname, current_user->name);
        printf("Do you really want to delete your profile? \nIt will not be possible to change. (Y/N)\n");
        c = getch();
        if (c == 'y' || c == 'Y') { DeleteUserInBase(current_user->login); current_user = DeleteListOfUser(current_user); system("cls"); exit(0);}
        if (c == 'n' || c == 'N') { return; }
    }
}

// Service functions
struct User Initializing_User(char *login, char *password, char *name, char *surname, char *father, int *date_birth, int *date_reg, char *phone, char *city, ull c, boolean bl_l, struct User *next, boolean MFD)
{
    struct User user;
    if (login != NULL) {strcpy(user.login, login);} else memset(user.login, 0, STR_LEN);
    if (password != NULL) {strcpy(user.password, password);} else memset(user.password, 0, STR_LEN);
    if (name != NULL) {strcpy(user.name, name);} else memset(user.name, 0, STR_LEN);
    if (surname != NULL) {strcpy(user.surname, surname);} else memset(user.surname, 0, STR_LEN);
    if (father != NULL) {strcpy(user.father, father);} else memset(user.father, 0, STR_LEN);
    if (date_birth != NULL) { register int i = 0; while (i < DATE_LEN) { user.date_birth[i]=date_birth[i]; i++;} } else memset(user.date_birth, 0, DATE_LEN*sizeof(int));
    if (date_reg != NULL) { register int i = 0; while (i < DATE_LEN) { user.date_reg[i]=date_reg[i]; i++;} } else memset(user.date_reg, 0, DATE_LEN*sizeof(int));
    if (phone != NULL) {strcpy(user.phone, phone);} else memset(user.phone, 0, STR_LEN);
    if (city != NULL) {strcpy(user.city, city);} else memset(user.city, 0, STR_LEN);
    user.count_of_orders = c;
    user.blacklist = bl_l;
    user.next = next;
    user.mark_for_delete = MFD;
    user.money = 0;
    return user;
};
struct Order Initializing_Order(ull id, int *date, ull price, char *login, struct Order *next, boolean MFD)
{
    struct Order order;
    order.ID = id;
    if (date != NULL) {register int i = 0; while (i < DATE_LEN) { order.date[i] = date[i]; i++; }} else memset(order.date, 0, DATE_LEN*sizeof(int));
    if (login != NULL) {strcpy(order.login, login);} else memset(order.login, 0, STR_LEN);
    order.next = next;
    order.mark_for_delete = MFD;
    order.price = price;
    return order;
};
struct Goods Initializing_Goods(ull id, char *name, ull price, enum Size size, char *country, char *producer, ull id_order, struct Goods *next, boolean MFD, ull cnt_goods)
{
    struct Goods goods;
    goods.ID = id;
    if (name != NULL) {strcpy(goods.name, name);} else memset(goods.name,0,STR_LEN);
    goods.price = price;
    goods.size = size;
    if (country != NULL) {strcpy(goods.country, country);} else memset(goods.country,0,STR_LEN);
    if (producer != NULL) {strcpy(goods.producer, producer);} else memset(goods.producer,0,STR_LEN);
    goods.ID_order = id_order;
    goods.next = next;
    goods.mark_for_delete = MFD;
    goods.count_goods = cnt_goods;
    return goods;
};
struct Assortment Initializing_Assortment(ull id, char *name, ull price, enum Size size, boolean exist, char *country, char *producer, struct Assortment *next, boolean MFD)
{
    struct Assortment assortment;
    assortment.ID = id;
    if (name != NULL) {strcpy(assortment.name, name);} else memset(assortment.name,0,STR_LEN);
    assortment.price = price;
    assortment.size = size;
    assortment.exist = exist;
    if (country != NULL) {strcpy(assortment.country, country);} else memset(assortment.country,0,STR_LEN);
    if (producer != NULL) {strcpy(assortment.producer, producer);} else memset(assortment.producer,0,STR_LEN);
    assortment.next = next;
    assortment.mark_for_delete = MFD;
    return assortment;
};


ull CreateID(enum Type type)
{
    ull res;
    switch (type)
    {
        case user_type: {res = 0;} break;
        case order_type:{lastID[0]++; res = lastID[0];} break;
        case goods_type:{lastID[1]++; res = lastID[1];} break;
        case assortment_type:{lastID[2]++; res = lastID[2];}
    }
    return res;
}


struct User* AddUserToList(struct User *head, struct User user)
{
    struct User *temp = malloc(sizeof(struct User));
    *temp = user;
    temp->next = head;
    return temp;
};
struct Order* AddOrderToList(struct Order *head, struct Order order)
{
    struct Order *temp = malloc(sizeof(struct Order));
    *temp = order;
    temp->next = head;
    return temp;
};
struct Goods* AddGoodsToList(struct Goods *head, struct Goods goods)
{
    struct Goods *temp = malloc(sizeof(struct Goods));
    *temp = goods;
    temp->next = head;
    return temp;
};
struct Assortment* AddAssortmentToList(struct Assortment *head, struct Assortment assortment)
{
    struct Assortment *temp = malloc(sizeof(struct Assortment));
    *temp = assortment;
    temp->next = head;
    return temp;
};


struct User* DeleteListOfUser(struct User *head)
{
    while (head != NULL)
    {
        struct User *p = head;
        head=head->next;
        free(p);
    }
    return NULL;
}
struct Order* DeleteListOfOrder(struct Order *head)
{
    while (head != NULL)
    {
        struct Order *p = head;
        head=head->next;
        free(p);
    }
    return NULL;
}
struct Goods* DeleteListOfGoods(struct Goods *head)
{
    while (head != NULL)
    {
        struct Goods *p = head;
        head=head->next;
        free(p);
    }
    return NULL;
}
struct Assortment* DeleteListOfAssortment(struct Assortment *head)
{
    while (head != NULL)
    {
        struct Assortment *p = head;
        head=head->next;
        free(p);
    }
    return NULL;
}


struct User* DeleteElemListOfUser(struct User *head, struct User *cur)
{
    if (head == cur)
    {
        head=head->next;
        free(cur);
        return head;
    }
    struct User *p = head;
    while (p->next != cur) p=p->next;
    p->next = p->next->next;
    free(cur);
    return head;
};
struct Order* DeleteElemListOfOrder(struct Order *head, struct Order *cur)
{
    if (head == cur)
    {
        head=head->next;
        free(cur);
        return head;
    }
    struct Order *p = head;
    while (p->next != cur) p=p->next;
    p->next = p->next->next;
    free(cur);
    return head;
};
struct Goods* DeleteElemListOfGoods(struct Goods *head, struct Goods *cur)
{
    if (head == cur)
    {
        head=head->next;
        free(cur);
        return head;
    }
    struct Goods *p = head;
    while (p->next != cur) p=p->next;
    p->next = p->next->next;
    free(cur);
    return head;
};
struct Assortment* DeleteElemListOfAssortment(struct Assortment *head, struct Assortment *cur)
{
    if (head == cur)
    {
        head=head->next;
        free(cur);
        return head;
    }
    struct Assortment *p = head;
    while (p->next != cur) p=p->next;
    p->next = p->next->next;
    free(cur);
    return head;
};


void AddUserToBase(struct User user)
{
    user.next = NULL;
    FILE * f = fopen("user.data","a+b");
    fwrite(&user, sizeof(struct User), 1, f);
    fclose(f);
}
void AddOrderToBase(struct Order order)
{
    order.next = NULL;
    FILE * f = fopen("order.data","a+b");
    fwrite(&order, sizeof(struct Order), 1, f);
    fclose(f);
}
void AddGoodsToBase(struct Goods goods)
{
    goods.next = NULL;
    FILE * f = fopen("goods.data","a+b");
    fwrite(&goods, sizeof(struct Goods), 1, f);
    fclose(f);
}
void AddAssortmentToBase(struct Assortment assortment)
{
    assortment.next = NULL;
    FILE * f = fopen("assortment.data","a+b");
    fwrite(&assortment, sizeof(struct Assortment), 1, f);
    fclose(f);
}


struct User* FindUserInBase(struct User user)
{
    struct User *list = NULL;
    struct User temp;
    FILE *f = fopen("user.data", "rb");
    while (fread(&temp, sizeof(struct User), 1, f))
    {
        if (  ( temp.mark_for_delete == 0)
            &&(!strcmp(user.login, temp.login)                  || user.login[0] == 0)
            &&(!strcmp(user.password, temp.password)            || user.password[0] == 0)
            &&(!strcmp(user.name, temp.name)                    || user.name[0] == 0)
            &&(!strcmp(user.surname, temp.surname)              || user.surname[0] == 0)
            &&(!strcmp(user.father, temp.father)                || user.father[0] == 0)
            &&(((user.date_birth[0] == temp.date_birth[0]) && (user.date_birth[1] == temp.date_birth[1]) && (user.date_birth[2] == temp.date_birth[2])) || user.date_birth[0] == 0)
            &&(((user.date_reg[0] == temp.date_reg[0]) && (user.date_reg[1] == temp.date_reg[1]) && (user.date_reg[2] == temp.date_reg[2])) || user.date_reg[0] == 0)
            &&(!strcmp(user.phone, temp.phone)                  || user.phone[0] == 0)
            &&(!strcmp(user.city, temp.city)                    || user.city[0] == 0)
            &&((user.count_of_orders == temp.count_of_orders)   ||(user.count_of_orders == 0))
            &&((user.blacklist == temp.blacklist)               ||(user.blacklist == 0))
            &&((user.money == temp.blacklist)                   ||(user.money == 0))
           )
            {
                list = AddUserToList(list, temp);
            }
    }
    fclose(f);
    return list;
};
struct Order* FindOrderInBase(struct Order order)
{
    struct Order *list = NULL;
    struct Order temp;
    FILE *f = fopen("order.data", "rb");
    while (fread(&temp, sizeof(struct Order), 1, f))
    {
        if (  ( temp.mark_for_delete == 0)
            &&((order.ID == temp.ID)                || order.ID == 0)
            &&(!strcmp(order.login, temp.login)     || order.login[0] == 0)
            &&(((order.date[0] == temp.date[0]) && (order.date[1] == temp.date[1]) && (order.date[2] == temp.date[2])) || order.date[0] == 0)
           )
        {
                list = AddOrderToList(list, temp);
        }
    }
    fclose(f);
    return list;
};
struct Goods* FindGoodsInBase(struct Goods goods)
{
    struct Goods *list = NULL;
    struct Goods temp;
    FILE *f = fopen("goods.data", "rb");
    while (fread(&temp, sizeof(struct Goods), 1, f))
    {
        if (  ( temp.mark_for_delete == 0)
            &&((goods.ID == temp.ID)                    || goods.ID == 0)
            &&(!strcmp(goods.name, temp.name)           || goods.name[0] == 0)
            &&((goods.price == temp.price)              || goods.price == 0)
            &&((goods.size == temp.size)                || goods.size == NullSize)
            &&(!strcmp(goods.country, temp.country)     || goods.country[0] == 0)
            &&(!strcmp(goods.producer, temp.producer)   || goods.producer[0] == 0)
            &&((goods.ID_order == temp.ID_order)        || goods.ID_order == 0)
           )
        {
                list = AddGoodsToList(list, temp);
        }
    }
    fclose(f);
    return list;
};
struct Assortment* FindAssortmentInBase(struct Assortment assortment)
{
    struct Assortment *list = NULL;
    struct Assortment temp;
    FILE *f = fopen("assortment.data", "rb");
    while (fread(&temp, sizeof(struct Assortment), 1, f))
    {
        if (  ( temp.mark_for_delete == 0)
            &&((assortment.ID == temp.ID)                    || assortment.ID == 0)
            &&(!strcmp(assortment.name, temp.name)           || assortment.name[0] == 0)
            &&((assortment.price == temp.price)              || assortment.price == 0)
            &&((assortment.size == temp.size)                || assortment.size == NullSize)
            &&(!strcmp(assortment.country, temp.country)     || assortment.country[0] == 0)
            &&(!strcmp(assortment.producer, temp.producer)   || assortment.producer[0] == 0)
            &&((assortment.exist == temp.exist)              || assortment.exist == 0)
           )
        {
                list = AddAssortmentToList(list, temp);
        }
    }
    fclose(f);
    return list;
};


void EditUserBase(struct User user)
{
    user.next = NULL;
    FILE *f = fopen("user.data", "r+b");
    struct User temp;
    while (fread(&temp, sizeof(struct User), 1, f))
    {
        if (!strcmp(temp.login, user.login)) break;
    }
    fseek(f,-sizeof(struct User), SEEK_CUR);
    fwrite(&user, sizeof(struct User), 1, f);
    fclose(f);
}
void EditOrderBase(struct Order order)
{
    order.next = NULL;
    FILE *f = fopen("order.data", "r+b");
    struct Order temp;
    while (fread(&temp, sizeof(struct Order), 1, f))
    {
        if (temp.ID == order.ID) break;
    }
    fseek(f,-sizeof(struct Order), SEEK_CUR);
    fwrite(&order, sizeof(struct Order), 1, f);
    fclose(f);
}
void EditGoodsBase(struct Goods goods)
{
    goods.next = NULL;
    FILE *f = fopen("goods.data", "r+b");
    struct Goods temp;
    while (fread(&temp, sizeof(struct Goods), 1, f))
    {
        if (temp.ID == goods.ID) break;
    }
    fseek(f,-sizeof(struct Goods), SEEK_CUR);
    fwrite(&goods, sizeof(struct Goods), 1, f);
    fclose(f);
}
void EditAssortmentBase(struct Assortment assortment)
{
    assortment.next = NULL;
    FILE *f = fopen("assortment.data", "r+b");
    struct Assortment temp;
    while (fread(&temp, sizeof(struct Assortment), 1, f))
    {
        if (temp.ID == assortment.ID) break;
    }
    fseek(f,-sizeof(struct Assortment), SEEK_CUR);
    fwrite(&assortment, sizeof(struct Assortment), 1, f);
    fclose(f);
}


void DeleteUserInBase(char * login)
{
    struct User temp;
    strcpy(temp.login, login);
    temp.mark_for_delete = 1;
    EditUserBase(temp);
}
void DeleteOrderInBase(ull id)
{
    struct Order temp;
    temp.ID = id;
    temp.mark_for_delete = 1;
    EditOrderBase(temp);
}
void DeleteGoodsInBase(ull id)
{
    struct Goods temp;
    temp.ID = id;
    temp.mark_for_delete = 1;
    EditGoodsBase(temp);
}
void DeleteAssortmentInBase(ull id)
{
    struct Assortment temp;
    temp.ID = id;
    temp.mark_for_delete = 1;
    EditAssortmentBase(temp);
}


void ViewAllUsers(boolean bl_l)
{
    FILE *f = fopen("user.data", "rb");
    struct User temp;
    int i = 1;
    while (fread(&temp, sizeof(struct User), 1, f))
    {
        if (temp.blacklist != bl_l || temp.mark_for_delete == 1) continue;
        printf("%i)  Login:  %s\n",i , temp.login);
        printf("     Name:  %s %s\n", temp.surname, temp.name);
        printf("     City:  %s\n", temp.city);
        printf("    Phone:  %s\n\n", temp.phone);
        i++;
    }
    fclose(f);
}
void ViewOrderUsers(char * log)
{
    FILE *f = fopen("order.data", "r+b");
    struct Order temp;
    int i = 1;
    fseek(f,-sizeof(struct Order), SEEK_END);
    while (fread(&temp, sizeof(struct Order), 1, f))
    {
        if (strcmp(log, "admin")) if (strcmp(log, temp.login)) {if (fseek(f,-2*sizeof(struct Order), SEEK_CUR)) break; continue;}
        struct Goods *list = FindGoodsInBase(Initializing_Goods(0, NULL, 0, NullSize, NULL, NULL, temp.ID, NULL, 0, 0));
        struct Goods *tmp = list;
        printf("%i)   ID order: %u\n", i, temp.ID);
        printf("         User: %s\n", temp.login);
        printf("        Price: %u\n", temp.price);
        printf("         Date: %i.%i.%i\n", temp.date[0], temp.date[1], temp.date[2]);
        printf("        Goods: \n");
        while (tmp != NULL)
        {
            printf("              *%s\n", tmp->name);
            tmp = tmp->next;
        }
        printf("\n\n");
        list = DeleteListOfGoods(list);
        if (fseek(f,-2*sizeof(struct Order), SEEK_CUR)) break;
        i++;
    }
    fclose(f);
}
void ViewAssortment(void)
{
    FILE *f = fopen("assortment.data", "rb");
    struct Assortment temp;
    int i = 1;
    while (fread(&temp, sizeof(struct Assortment), 1, f))
    {
        if (temp.mark_for_delete == 1) continue;
        printf("%i) ID: %u\n", i, temp.ID);
        printf("    Name: %s\n", temp.name);
        printf("    Producer: %s\n", temp.producer);
        printf("    Country: %s\n", temp.country);
        printf("    Price: %u\n", temp.price);
        printf("    Exist: %s\n\n", (temp.exist) ? "+" : "-");
        i++;
    }
    fclose(f);
}


void ClearDataBases(void)
{
    //User
    FILE *f1 = fopen("user.data", "rb");
    FILE *f2 = fopen("_user.data", "wb");
    struct User user;
    while (fread(&user, sizeof(struct User), 1, f1))
    {
        if (user.mark_for_delete == 0) fwrite(&user, sizeof(struct User), 1, f2);
    }
    fclose(f1); fclose(f2);
    remove("user.data");
    rename("_user.data", "user.data");

    // Order
    f1 = fopen("order.data", "rb");
    f2 = fopen("_order.data", "wb");
    struct Order order;
    while (fread(&order, sizeof(struct Order), 1, f1))
    {
         if (order.mark_for_delete == 0) fwrite(&order, sizeof(struct Order), 1, f2);
    }
    fclose(f1); fclose(f2);
    remove("order.data");
    rename("_order.data", "order.data");

    // Goods
    f1 = fopen("goods.data", "rb");
    f2 = fopen("_goods.data", "wb");
    struct Goods goods;
    while (fread(&goods, sizeof(struct Goods), 1, f1))
    {
        if (goods.mark_for_delete == 0) fwrite(&goods, sizeof(struct Goods), 1, f2);
    }
    fclose(f1); fclose(f2);
    remove("goods.data");
    rename("_goods.data", "goods.data");

    //Assortment
    f1 = fopen("assortment.data", "rb");
    f2 = fopen("_assortment.data", "wb");
    struct Assortment assortment;
    while (fread(&assortment, sizeof(struct Assortment), 1, f1))
    {
        if (assortment.mark_for_delete == 0) fwrite(&assortment, sizeof(struct Assortment), 1, f2);
    }
    fclose(f1); fclose(f2);
    remove("assortment.data");
    rename("_assortment.data", "assortment.data");
}
void DeleteDataBases(void)
{
    remove("user.data");
    remove("order.data");
    remove("goods.data");
    remove("assortment.data");
    FILE *f = fopen("user.data", "wb"); fclose(f);
    f = fopen("order.data", "wb"); fclose(f);
    f = fopen("goods.data", "wb"); fclose(f);
    f = fopen("assortment.data", "wb"); fclose(f);
    lastID[0] = 0; lastID[1] = 0; lastID[2] = 0;
}
