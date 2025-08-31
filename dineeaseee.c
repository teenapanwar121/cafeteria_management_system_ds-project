#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

// Structure for menu items
struct Item {
    char name[50];
    float rating;
    float price;
    int food_Id_No;
    struct Item *next;
    struct Item *prev;
};

// Structure for order history
struct order_hist {
    int index; // Unique index for the order
    int Customer_ID;
    int items[10][2];
    float amount;
    char date[11]; // Store the date of the order
    char customerName[25]; // Store customer name
    int isTeacher; // 1 for teacher, 0 for student
    struct order_hist *next; // Next order in the global list
    struct order_hist *prev; // Previous order in the global list
    struct order_hist *nextOrder; // Next order by the same customer
};

// Global variables
int cust_id = 1;
int order_index = 1; // Unique index for orders
char last_order_date[11] = ""; // Track the last order's date
struct Item* head = NULL;
struct Item* last = NULL;
int Today_customer = 0;
float total_income = 0;

// Separate lists for teachers and students
struct order_hist* teacherList = NULL; // Head of the teachers' list
struct order_hist* teacherListLast = NULL; // Last of the teachers' list
struct order_hist* studentList = NULL; // Head of the students' list
struct order_hist* studentListLast = NULL; // Last of the students' list

// Function to create a new menu item node
struct Item* getnewnode(char a[], float p, int fin) {
    struct Item* temp = (struct Item*)malloc(sizeof(struct Item));
    temp->food_Id_No = fin;
    strcpy(temp->name, a);
    temp->rating = 4.0;
    temp->price = p;
    temp->next = NULL;
    temp->prev = NULL;
    return temp;
}

// Function to insert a new menu item
void insert(char n[], float p, int fin) {
    struct Item* temp1 = getnewnode(n, p, fin);
    if (head == NULL) {
        head = temp1;
        last = temp1;
    } else {
        temp1->prev = last;
        last->next = temp1;
        last = temp1;
    }
}

// Function to create a new order history node
struct order_hist* getnewNode_hist() {
    struct order_hist* temp = (struct order_hist*)malloc(sizeof(struct order_hist));
    temp->next = temp->prev = temp->nextOrder = NULL;
    temp->isTeacher = 0; // Default to student
    temp->index = order_index++; // Assign a unique index
    return temp;
}

// Function to display the menu
void Display() {
    printf("                                      -----------------                                                     \n");
    printf("---------------------------------------------------MENU--------------------------------------------------------\n");
    printf("                                             --------------------                                              \n");
    printf("INDEX      ITEM NAME      PRICE        RATING\n");
    struct Item* temp = head;
    if (head == NULL) {
        printf("empty\n");
    }
    while (temp != NULL) {
        printf("%d\t%s\t%f\t%f\n", temp->food_Id_No, temp->name, temp->price, temp->rating);
        temp = temp->next;
    }
    printf("------------------------------------------------------------------------------------------------------------------------------\n");
}

// Function to handle login
int login() {
    char username[20];
    char userpwd[11]; //for storing pwd
    printf("enter your username : ");
    scanf("%s", username);

    printf("enter password: ");
    for (int i = 0; i < 10; i++) {
        userpwd[i] = getch();
        printf("*");
    }
    userpwd[10] = '\0';

    if (!strcmp(username, "Admin") && !strcmp(userpwd, "MCA2SEMPBL")) {
        printf("\n\nLogged in successfully\n");
        return 1;
    } else {
        printf("\n\nIncorrect username and password\n");
        return 0;
    }
}

// Function to add an order to the teachers' list
void addToTeacherList(struct order_hist* order) {
    if (teacherList == NULL) {
        teacherList = teacherListLast = order;
    } else {
        teacherListLast->next = order;
        order->prev = teacherListLast;
        teacherListLast = order;
    }
}

// Function to add an order to the students' list
void addToStudentList(struct order_hist* order) {
    if (studentList == NULL) {
        studentList = studentListLast = order;
    } else {
        studentListLast->next = order;
        order->prev = studentListLast;
        studentListLast = order;
    }
}

// Function to find the last order by a customer
struct order_hist* findLastOrderByCustomer(struct order_hist* list, char* customerName) {
    struct order_hist* temp = list;
    while (temp != NULL) {
        if (strcmp(temp->customerName, customerName) == 0) {
            while (temp->nextOrder != NULL) {
                temp = temp->nextOrder;
            }
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Function to display orders for a specific date
void displayOrdersByDate(char date[]) {
    printf("----------------------------------------------ORDERS FOR DATE: %s-----------------------------------------------\n", date);
    printf("INDEX   TYPE   CUSTOMER NAME      TOTAL_AMOUNT\n");

    // Display teachers' orders for this date
    struct order_hist* temp = teacherList;
    while (temp != NULL) {
        struct order_hist* customerOrder = temp;
        while (customerOrder != NULL) {
            if (strcmp(customerOrder->date, date) == 0) {
                printf("%d\tTeacher\t%s\t%f\n", customerOrder->index, customerOrder->customerName, customerOrder->amount);
            }
            customerOrder = customerOrder->nextOrder;
        }
        temp = temp->next;
    }

    // Display students' orders for this date
    temp = studentList;
    while (temp != NULL) {
        struct order_hist* customerOrder = temp;
        while (customerOrder != NULL) {
            if (strcmp(customerOrder->date, date) == 0) {
                printf("%d\tStudent\t%s\t%f\n", customerOrder->index, customerOrder->customerName, customerOrder->amount);
            }
            customerOrder = customerOrder->nextOrder;
        }
        temp = temp->next;
    }

    printf("-------------------------------------------------------------------------------------------------------------\n");
}

// Function to calculate total income for a specific date
float calculateTotalIncomeByDate(char date[]) {
    float total_income_for_date = 0.0;

    // Traverse teachers' orders
    struct order_hist* temp = teacherList;
    while (temp != NULL) {
        struct order_hist* customerOrder = temp;
        while (customerOrder != NULL) {
            if (strcmp(customerOrder->date, date) == 0) {
                total_income_for_date += customerOrder->amount;
            }
            customerOrder = customerOrder->nextOrder;
        }
        temp = temp->next;
    }

    // Traverse students' orders
    temp = studentList;
    while (temp != NULL) {
        struct order_hist* customerOrder = temp;
        while (customerOrder != NULL) {
            if (strcmp(customerOrder->date, date) == 0) {
                total_income_for_date += customerOrder->amount;
            }
            customerOrder = customerOrder->nextOrder;
        }
        temp = temp->next;
    }

    return total_income_for_date;
}

// Function to calculate total number of customers for a specific date
int calculateTotalCustomersByDate(char date[]) {
    int total_customers_for_date = 0;

    // Traverse teachers' orders
    struct order_hist* temp = teacherList;
    while (temp != NULL) {
        struct order_hist* customerOrder = temp;
        while (customerOrder != NULL) {
            if (strcmp(customerOrder->date, date) == 0) {
                total_customers_for_date++;
            }
            customerOrder = customerOrder->nextOrder;
        }
        temp = temp->next;
    }

    // Traverse students' orders
    temp = studentList;
    while (temp != NULL) {
        struct order_hist* customerOrder = temp;
        while (customerOrder != NULL) {
            if (strcmp(customerOrder->date, date) == 0) {
                total_customers_for_date++;
            }
            customerOrder = customerOrder->nextOrder;
        }
        temp = temp->next;
    }

    return total_customers_for_date;
}

// Function to delete an order by index
void deleteOrder(int index) {
    struct order_hist* temp = teacherList;
    while (temp != NULL) {
        struct order_hist* customerOrder = temp;
        while (customerOrder != NULL) {
            if (customerOrder->index == index) {
                // Subtract the order amount from total income
                total_income -= customerOrder->amount;

                // Decrement the total number of customers
                Today_customer--;

                // Adjust pointers to remove the order
                if (customerOrder->prev != NULL) {
                    customerOrder->prev->next = customerOrder->next;
                } else {
                    teacherList = customerOrder->next;
                }
                if (customerOrder->next != NULL) {
                    customerOrder->next->prev = customerOrder->prev;
                }
                free(customerOrder);
                printf("Order with index %d deleted successfully.\n", index);
                return;
            }
            customerOrder = customerOrder->nextOrder;
        }
        temp = temp->next;
    }

    temp = studentList;
    while (temp != NULL) {
        struct order_hist* customerOrder = temp;
        while (customerOrder != NULL) {
            if (customerOrder->index == index) {
                // Subtract the order amount from total income
                total_income -= customerOrder->amount;

                // Decrement the total number of customers
                Today_customer--;

                // Adjust pointers to remove the order
                if (customerOrder->prev != NULL) {
                    customerOrder->prev->next = customerOrder->next;
                } else {
                    studentList = customerOrder->next;
                }
                if (customerOrder->next != NULL) {
                    customerOrder->next->prev = customerOrder->prev;
                }
                free(customerOrder);
                printf("Order with index %d deleted successfully.\n", index);
                return;
            }
            customerOrder = customerOrder->nextOrder;
        }
        temp = temp->next;
    }

    printf("Order with index %d not found.\n", index);
}

// Function to place an order
void order() {
    int a[10][2];
    int n, j = 0, i = 0;

    // Ask if the user is a student or teacher
    int isTeacher;
    printf("Are you a student or teacher?\n1. Student\n2. Teacher\n");
    scanf("%d", &isTeacher);

    char name[25];
    printf("Enter your name: ");
    scanf("%s", name);

    // Check if the customer has previous orders
    struct order_hist* lastOrder = NULL;
    if (isTeacher == 2) {
        lastOrder = findLastOrderByCustomer(teacherList, name);
    } else {
        lastOrder = findLastOrderByCustomer(studentList, name);
    }

    int linkToIndex = -1;
    if (lastOrder != NULL) {
        printf("You have previous orders. Do you want to link this order to your previous index? (1 for Yes, 0 for No): ");
        scanf("%d", &linkToIndex);
        if (linkToIndex == 1) {
            printf("Your previous order index is: %d\n", lastOrder->index);
        }
    }

    do {
        printf("Please enter food id number of item and its quantity: ");
        for (i = 0; i < 2; i++) {
            scanf("%d", &a[j][i]);
        }
        j++;
        printf("Do you want more items?\n1. Yes\n2. No: ");
        scanf("%d", &n);
    } while (n == 1);

    float total_amount = 0.0;
    char Date[11];
    printf("Enter today's date (dd-mm-yyyy): ");
    scanf("%s", Date);

    // Reset order index if it's a new day
    if (strcmp(Date, last_order_date) != 0) {
        order_index = 1; // Reset index for the new day
        strcpy(last_order_date, Date); // Update last order date
    }

    printf("\n------------------------------------------------------------------------------------------------------------------------\n");
    printf("                                           BILL                                       \n");
    printf("Name: %s\n", name);
    printf("Date: %s\n", Date);

    for (int k = 0; k < j; k++) {
        struct Item* temp = head;
        while (temp->food_Id_No != a[k][0]) {
            temp = temp->next;
        }
        printf("------------------------------------------------------------------------------------------------------------------------\n");
        printf("%d\t%s\t%d\t%f\n", temp->food_Id_No, temp->name, a[k][1], (a[k][1] * (temp->price)));
        total_amount += (a[k][1] * (temp->price));
    }

    printf("-------------------------------------------------------------------------------------------------------------\n");
    printf("Total payable amount is: %f\n", total_amount);
    printf("-------------------------------------------------------------------------------------------------------------\n");

    // Create a new order
    struct order_hist* temp2 = getnewNode_hist();
    temp2->amount = total_amount;
    temp2->Customer_ID = cust_id++;
    temp2->isTeacher = (isTeacher == 2) ? 1 : 0; // Set isTeacher flag
    strcpy(temp2->customerName, name); // Store customer name
    strcpy(temp2->date, Date);

    // Link this order to previous orders by the same customer
    if (linkToIndex == 1 && lastOrder != NULL) {
        temp2->index = lastOrder->index; // Use the same index as the previous order
        lastOrder->nextOrder = temp2;
    } else {
        if (isTeacher == 2) {
            addToTeacherList(temp2); // Add to teachers' list
        } else {
            addToStudentList(temp2); // Add to students' list
        }
    }

    Today_customer++;
    total_income += total_amount;

    // Display all orders grouped by date
    displayOrdersByDate(Date);
}

// Main function
int main() {
    head = NULL;
    last = NULL;
    insert("  Burger      ", 70.23, 1);
    insert("  Pizza       ", 237.26, 6);
    insert("  HOT CAKE    ", 750.23, 10);
    insert("  Coffee      ", 70.23, 2);
    insert("  Ice-cream    ", 70.23, 3);
    insert("  Sandwich     ", 60.23, 4);
    insert("  Momo        ", 70.3, 8);
    insert("  Bread       ", 35.13, 5);
    insert("  Cold Drinks  ", 20.13, 9);

    int choice;
    do {
        printf("---------------------------------------------------------------\n");
        printf("1..... Food Part\n");
        printf("2..... Admin Panel\n");
        printf("3..... Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int ex;
                do {
                    printf("1. FOOD LIST\n");
                    printf("2. ORDER\n");
                    int n;
                    scanf("%d", &n);
                    switch (n) {
                        case 1:
                            Display();
                            break;
                        case 2:
                            order();
                            break;
                    }
                    printf("1. FOOD PART\n");
                    printf("2. MAIN MENU\n");
                    scanf("%d", &ex);
                } while (ex == 1);
                break;
            }
            case 2: {
                int k = login();
                if (k == 1) {
                    int ch;
                    do {
                        printf("1. Enter new dish\n");
                        printf("2. Total Income for a Date\n");
                        printf("3. Total Customers for a Date\n");
                        printf("4. Display Orders for a Date\n");
                        printf("5. Delete an Order\n");
                        printf("6. List of items\n");
                        printf("7. Exit\n");
                        printf("Enter your choice: ");
                        scanf("%d", &ch);

                        switch (ch) {
                            case 1: {
                                float p;
                                int fin;
                                char n[50];
                                printf("Enter the name of item: ");
                                scanf("%s", n);
                                printf("Enter the price of item: ");
                                scanf("%f", &p);
                                printf("Enter the food_id_no of item: ");
                                scanf("%d", &fin);
                                insert(n, p, fin);
                                printf("New dish added successfully.\n");
                                break;
                            }
                            case 2: {
                                char date[11];
                                printf("Enter the date (dd-mm-yyyy): ");
                                scanf("%s", date);
                                float income = calculateTotalIncomeByDate(date);
                                printf("Total income for %s: %f\n", date, income);
                                break;
                            }
                            case 3: {
                                char date[11];
                                printf("Enter the date (dd-mm-yyyy): ");
                                scanf("%s", date);
                                int customers = calculateTotalCustomersByDate(date);
                                printf("Total customers for %s: %d\n", date, customers);
                                break;
                            }
                            case 4: {
                                char date[11];
                                printf("Enter the date (dd-mm-yyyy): ");
                                scanf("%s", date);
                                displayOrdersByDate(date);
                                break;
                            }
                            case 5: {
                                int index;
                                printf("Enter the index of the order to delete: ");
                                scanf("%d", &index);
                                deleteOrder(index);
                                break;
                            }
                            case 6:
                                Display();
                                break;
                        }
                    } while (ch != 7);
                }
                break;
            }
        }
    } while (choice != 3);
    return 0;
}
