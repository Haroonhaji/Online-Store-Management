#include<iostream>
#include<string>
using namespace std;

// Struct to represent a Product node in the BST
struct Product{
    int productId;
    string name;
    double price;
    int quantity;
    Product* left;
    Product* right;

    Product(int id, string n, double p, int q) : productId(id), name(n), price(p), quantity(q), left(NULL), right(NULL) {}
};

// Class to manage products using Binary Search Tree (BST)
class ProductBST{
private:
    Product* root;

    // Recursive helper to insert a new product in BST
    Product* insertProduct(Product* root, int productId, string name, double price, int quantity){
        if(root==NULL)
            return new Product(productId, name, price, quantity);

        if(productId < root->productId)
            root->left=insertProduct(root->left, productId, name, price, quantity);
        else if(productId > root->productId)
            root->right=insertProduct(root->right, productId, name, price, quantity);

        return root;
    }

    // Recursive preorder search by product name
    Product* findProductByName(Product* root, string name){
        if(root == NULL)
            return NULL;
        if(root->name == name)
            return root;

        Product* left=findProductByName(root->left, name);
        if (left) return left;
        return findProductByName(root->right, name);
    }

    // Recursive preorder search by product ID
    Product* findProductByID(Product* root, int id){
        if(root == NULL)
            return NULL;
        if(root->productId == id)
            return root;

        Product* left=findProductByID(root->left, id);
        if (left) return left;
        return findProductByID(root->right, id);
    }

    // Recursive inorder traversal to display all products
    void displayProducts(Product* root){
        if(root == NULL)
            return;
        displayProducts(root->left);
        cout<<"ID: "<<root->productId<<", Name: "<<root->name<<", Price: $"<<root->price<<", Quantity: "<<root->quantity<<endl;
        displayProducts(root->right);
    }

    // Helper to find minimum node (used in deletion)
    Product* findMin(Product* root){
        while(root && root->left != NULL)
            root=root->left;
        return root;
    }

    // Recursive function to delete a product from BST
    Product* deleteProduct(Product* root, int productId){
        if(root == NULL)
            return root;

        if(productId < root->productId)
            root->left = deleteProduct(root->left, productId);
        else if(productId > root->productId)
            root->right = deleteProduct(root->right, productId);
        else{
            // Node found
            if(root->left == NULL){
                Product* temp=root->right;
                delete root;
                return temp;
            }
            else if(root->right == NULL){
                Product* temp=root->left;
                delete root;
                return temp;
            }

            // Two children: replace with inorder successor
            Product* temp=findMin(root->right);
            root->productId=temp->productId;
            root->name=temp->name;
            root->price=temp->price;
            root->quantity=temp->quantity;

            root->right=deleteProduct(root->right, temp->productId);
        }
        return root;
    }

public:
    ProductBST() : root(NULL) {}

    // Public function to insert product
    void insertProduct(int productId, string name, double price, int quantity){
        root=insertProduct(root, productId, name, price, quantity);
    }

    // Update quantity of a product by ID
    void increase_stock(int id, int quant){
        Product* temp = findProductByID(root,id);
        temp->quantity =  quant;
    }

    // Public function to search product by name
    Product* findProductByName(string name){
        return findProductByName(root, name);
    }

    // Display all products in the store
    void displayProducts(){
        displayProducts(root);
    }

    // Delete a product by ID
    void deleteProduct(int productId){
        root=deleteProduct(root, productId);
    }
};

// Class to represent an Order
class Order{
public:
    int productId;
    string productName;
    int quantity;
    double price;
    double totalPrice;

    Order(int id, string name, int qty, double p) : productId(id), productName(name), quantity(qty), price(p) {
        totalPrice=price*quantity;
    }
};

// Class to manage Orders using a Queue
class OrderQueue{
private:
    struct Node{
        Order* order;
        Node* next;
        Node(Order* ord) : order(ord), next(NULL) {}
    };
    Node* front;
    Node* rear;

public:
    OrderQueue() : front(NULL), rear(NULL) {}

    // Add order to queue
    void enqueue(Order* order){
        Node* newNode=new Node(order);
        if(!rear){
            front=rear=newNode;
            return;
        }
        rear->next=newNode;
        rear=newNode;
    }

    // Remove order from front
    Order* dequeue(){
        if (!front) return NULL;
        Node* temp=front;
        Order* order=front->order;
        front=front->next;
        delete temp;
        if (!front) rear = NULL;
        return order;
    }

    // Display current orders in queue
    void displayOrders(){
        Node* temp = front;
        int total = 0;
        while(temp){
            cout<<"Product: "<<temp->order->productName<<", Rate: "<<temp->order->price<<", Quantity: "<<temp->order->quantity<<", Total Price: $"<<temp->order->totalPrice<<endl;
            total += temp->order->totalPrice;
            temp=temp->next;
        }
        cout<<"Total Price of all Orders: $"<<total<<endl;
    }

    // Check if queue is empty
    bool isEmpty(){
        return front==NULL;
    }
};

// Class to manage all past orders (checkout history)
class CheckoutHistory{
private:
    struct Node{
        Order* order;
        Node* next;
        Node(Order* ord) : order(ord), next(NULL) {}
    };
    Node* head;

public:
    CheckoutHistory() : head(NULL) {}

    // Add order to history
    void addOrderToHistory(Order* order){
        Node* newNode=new Node(order);
        newNode->next=head;
        head=newNode;
    }

    // Display checkout history
    void displayHistory(){
        Node* temp =head;
        cout<<"\nCheckout History:\n";
        while (temp){
            cout<<"Product: "<<temp->order->productName<<", Quantity: "<<temp->order->quantity<<", Total Price: $"<<temp->order->totalPrice<<endl;
            temp=temp->next;
        }
    }
};

// Admin class controls product management and history
class Admin{
private:
    int pin;
    CheckoutHistory checkoutHistory;

public:
    Admin() : pin(1234) {} // Default PIN

    bool validatePin(int enteredPin){
        return enteredPin==pin;
    }

    void changePin(int newPin){
        pin=newPin;
    }

    void viewCheckoutHistory(){
        checkoutHistory.displayHistory();
    }

    void addToCheckoutHistory(Order* order){
        checkoutHistory.addOrderToHistory(order);
    }
};

// Represents a user/customer
class User{
public:
    string name;
    OrderQueue orderQueue;

    User(string username) : name(username) {}

    // Place an order for a product
    void placeOrder(ProductBST& bst, string productName, int quantity){
        Product* product=bst.findProductByName(productName);
        if(product){
            if(product->quantity >= quantity){
                Order* order = new Order(product->productId, product->name, quantity, product->price);
                orderQueue.enqueue(order);
                product->quantity -= quantity; // Reduce available stock
                cout<<"Order placed: "<<product->name<<", Quantity: "<<quantity<<endl;
            }
            else{
                cout<<"Sorry! Insufficient stock!"<<endl;
            }
        } 
        else{
            cout<<"Sorry! No product by the name : "<<productName<<endl;
        }
    }

    // Display current orders
    void displayOrder(){
        cout<<"Your Orders:"<<endl;
        orderQueue.displayOrders();
    }

    // Final checkout, moves all orders to admin history
    void checkout(Admin& admin){
        cout<<"Checkout for "<<name<<endl;
        orderQueue.displayOrders();
        while(Order* order = orderQueue.dequeue()){
            admin.addToCheckoutHistory(order);
        }
    }
};

// Driver code: Main menu for admin and user interaction
int main(){
    ProductBST bst;
    Admin admin;
    User* user = NULL;
    
    // Add initial products
    bst.insertProduct(1, "laptop", 2000, 5);
    bst.insertProduct(2, "mobile", 1500, 7);
    bst.insertProduct(3, "headphone", 500, 3);

    int choice;
    while (true){
        cout<<"\n-----Online Store-----";
        cout<<"\n1. Login as Admin\n2. Login as User\n3. Exit\n";
        cout<<"Enter choice: ";
        cin>>choice;

        if(choice == 1){
            int pin;
            cout<<"Enter Admin PIN: ";
            cin>>pin;

            if(admin.validatePin(pin)){
                int adminChoice;
                while(true){
                    cout<<"\nAdmin Panel:\n";
                    cout<<"1. Add Product\n2. Delete Product\n3. Change stock quantity\n4. Display Products in store\n5. View Checkout History\n6. Change Pin\n7. Exit Admin Panel\n";
                    cout<<"Enter choice: ";
                    cin>>adminChoice;

                    if(adminChoice == 5){
                        admin.viewCheckoutHistory();
                    }
                    else if(adminChoice == 3){
                        int id , quant;
                        cout<<"Enter the Product ID of the Product whose stock you want to change: ";
                        cin>>id;
                        cout<<"Enter new quantity: ";
                        cin>>quant;
                        bst.increase_stock(id,quant);
                        cout<<"Quantity increased to "<<quant<<endl;
                    }
                    else if(adminChoice == 4) bst.displayProducts();
                    else if(adminChoice == 1){
                        int productId, quantity;
                        string name;
                        double price;

                        cout<<"Enter Product ID: ";
                        cin>>productId;
                        cout<<"Enter Product Name: ";
                        cin>>name;
                        cout<<"Enter Product Price: ";
                        cin>>price;
                        cout<<"Enter Product Quantity: ";
                        cin>>quantity;

                        bst.insertProduct(productId, name, price, quantity);
                        cout<<"Product added successfully.\n";
                    }
                    else if(adminChoice == 2){
                        int productId;
                        cout<<"Enter Product ID to delete: ";
                        cin>>productId;

                        bst.deleteProduct(productId);
                        cout<<"Product deleted successfully.\n";
                    }
                    else if(adminChoice == 6){
                        int newPin;
                        cout<<"Enter new PIN: ";
                        cin>>newPin;
                        admin.changePin(newPin);
                        cout<<"PIN changed successfully.\n";
                    }
                    else if (adminChoice == 7){
                        break;
                    } 
                    else{
                        cout<<"Invalid choice, try again.\n";
                    }
                }
            } 
            else{
                cout<<"Incorrect PIN."<<endl;
            }
        }
        else if(choice == 2){
            string UserName;
            cout<<"Enter your name: ";
            cin>>UserName;
            user=new User(UserName);

            int userChoice;
            while(true){
                cout<<"\nWelcome "<<UserName<<endl;
                cout<<"1. Display Products\n2. Search Product by Name\n3. Place an Order\n4. Display Orders\n5. Checkout\n6. Exit User Panel\n";
                cout<<"Enter choice: ";
                cin>>userChoice;

                if(userChoice == 1){
                    bst.displayProducts();
                }
                else if (userChoice == 2){
                    string productName;
                    cout<<"Enter product name: ";
                    cin>>productName;
                    Product* product=bst.findProductByName(productName);
                    if(product){
                        cout<<"Product Found: "<<product->name<<", Price: $"<<product->price<<", Quantity Available: "<<product->quantity<<endl;
                    } 
                    else{
                        cout<<"Product not found.\n";
                    }
                }
                else if(userChoice == 3){
                    string productName;
                    int quantity;
                    cout<<"Enter product name to order: ";
                    cin>>productName;
                    cout<<"Enter quantity: ";
                    cin>>quantity;

                    user->placeOrder(bst, productName, quantity);
                }
                else if(userChoice == 4){
                    user->displayOrder();
                }
                else if(userChoice == 5){
                    user->checkout(admin);
                    break;
                }
                else if(userChoice == 6){
                    break;
                }
                else{
                    cout<<"Invalid choice, try again.\n";
                }
            }
        }
        else if(choice == 3){
            break;
        }
        else{
            cout<<"Invalid choice, try again.\n";
        }
    }

    delete user; // Cleanup
    return 0;
}

