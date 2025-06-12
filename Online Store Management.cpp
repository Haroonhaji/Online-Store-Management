#include<iostream>
#include<string>
using namespace std;

struct Product{
    int productId;
    string name;
    double price;
    int quantity;
    Product* left;
    Product* right;

    Product(int id, string n, double p, int q) : productId(id), name(n), price(p), quantity(q), left(NULL), right(NULL) {}
};

class ProductBST{
private:
    Product* root;

    // Helper function to insert a product into the binary search tree
    Product* insertProduct(Product* root, int productId, string name, double price, int quantity){
        if(root==NULL)
            return new Product(productId, name, price, quantity);

        if(productId < root->productId)
            root->left=insertProduct(root->left, productId, name, price, quantity);
        else if(productId > root->productId)
            root->right=insertProduct(root->right, productId, name, price, quantity);

        return root;
    }

    // Helper function to find a product by name
    Product* findProductByName(Product* root, string name){
        if(root == NULL)
            return NULL;
        if(root->name == name)
            return root;

        Product* left=findProductByName(root->left, name);
        if (left) return left;
        return findProductByName(root->right, name);
    }

    // Helper function to display all products in the tree
    void displayProducts(Product* root){
        if(root == NULL)
            return;
        displayProducts(root->left);
        cout<<"ID: "<<root->productId<<", Name: "<<root->name<<", Price: $"<<root->price<<", Quantity: "<<root->quantity<<endl;
        displayProducts(root->right);
    }

    // Helper function to find the minimum value node in a tree
    Product* findMin(Product* root){
        while(root && root->left != NULL)root=root->left;
        return root;
    }

    // Helper function to delete a product from the tree
    Product* deleteProduct(Product* root, int productId){
        if(root == NULL){
		 return root;
		}

        if(productId < root->productId)
            root->left = deleteProduct(root->left, productId);
        else if(productId > root->productId)
            root->right = deleteProduct(root->right, productId);
        else{
            // Node to be deleted found
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

            // Node with two children: get the inorder successor (smallest in the right subtree)
            Product* temp=findMin(root->right);
            root->productId=temp->productId;
            root->name=temp->name;
            root->price=temp->price;
            root->quantity=temp->quantity;

            // Delete the inorder successor
            root->right=deleteProduct(root->right, temp->productId);
        }
        return root;
    }

public:
    ProductBST() : root(NULL){}

    // Public function to insert a product
    void insertProduct(int productId, string name, double price, int quantity){
        root=insertProduct(root, productId, name, price, quantity);
    }

    // Public function to find product by name
    Product* findProductByName(string name){
        return findProductByName(root, name);
    }

    // Public function to display all products
    void displayProducts(){
        displayProducts(root);
    }

    // Public function to delete a product
    void deleteProduct(int productId){
        root=deleteProduct(root, productId);
    }
};

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

class OrderQueue{
private:
    struct Node{
        Order* order;
        Node* next;
        Node(Order* ord) : order(ord), next(NULL){}
    };
    Node* front;
    Node* rear;

public:
    OrderQueue() : front(NULL), rear(NULL){}

    // Enqueue an order
    void enqueue(Order* order){
        Node* newNode=new Node(order);
        if(!rear){
            front=rear=newNode;
            return;
        }
        rear->next=newNode;
        rear=newNode;
    }

    // Dequeue an order
    Order* dequeue(){
        if (!front) return NULL;
        Node* temp=front;
        Order* order=front->order;
        front=front->next;
        delete temp;
        if (!front) rear = NULL;
        return order;
    }

    // Display all orders
    void displayOrders(){
        Node* temp = front;
        while(temp){
            cout<<"Product: "<<temp->order->productName<<", Quantity: "<<temp->order->quantity<<", Total Price: $"<<temp->order->totalPrice<<endl;
            temp=temp->next;
        }
    }

    // Check if the queue is empty
    bool isEmpty(){
        return front==NULL;
    }
};

class CheckoutHistory{
private:
    struct Node{
        Order* order;
        Node* next;
        Node(Order* ord) : order(ord), next(NULL){}
    };
    Node* head;

public:
    CheckoutHistory() : head(NULL){}

    // Add an order to checkout history
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

class Admin{
private:
    int pin;
    CheckoutHistory checkoutHistory;

public:
    Admin() : pin(1234){} // Default PIN

    // Validate PIN
    bool validatePin(int enteredPin){
        return enteredPin==pin;
    }

    // Change PIN
    void changePin(int newPin){
        pin=newPin;
    }

    // View checkout history
    void viewCheckoutHistory(){
        checkoutHistory.displayHistory();
    }

    // Add to checkout history
    void addToCheckoutHistory(Order* order){
        checkoutHistory.addOrderToHistory(order);
    }
};

class User{
public:
    string name;
    OrderQueue orderQueue;

    User(string username) : name(username){}

    // Place an order
    void placeOrder(ProductBST& bst, string productName, int quantity){
        Product* product=bst.findProductByName(productName);
        if(product && product->quantity >= quantity){
            Order* order = new Order(product->productId, product->name, quantity, product->price);
            orderQueue.enqueue(order);
            product->quantity -= quantity; // Reduce quantity in stock
            cout<<"Order placed: "<<product->name<<", Quantity: "<<quantity<<endl;
        } 
		else{
            cout<<"Product not available or insufficient stock!"<<endl;
        }
    }

    // Display order
    void displayOrder(){
        cout<<"Your Orders:"<<endl;
        orderQueue.displayOrders();
    }

    // Checkout
    void checkout(Admin& admin){
        cout<<"Checkout for "<<name<<endl;
        orderQueue.displayOrders();
        while(Order* order = orderQueue.dequeue()){
            admin.addToCheckoutHistory(order);
        }
    }
};

int main(){
    ProductBST bst;
    Admin admin;
    User* user = NULL;
    
    bst.insertProduct(234, "laptop", 2000, 5);
    bst.insertProduct(136, "mobile", 1500, 7);
    bst.insertProduct(512, "headphone", 500, 3);

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
                    cout<<"1. Add Product\n2. Delete Product\n3. View Checkout History\n4.Change Pin\n5. Exit Admin Panel\n";
                    cout<<"Enter choice: ";
                    cin>>adminChoice;

                    if(adminChoice == 4){
                    	cout<<endl;
                        int newPin;
                        cout<<"Enter new PIN: ";
                        cin>>newPin;
                        admin.changePin(newPin);
                        cout<<"PIN changed successfully.\n";
                    }
                    else if(adminChoice == 3){
                        admin.viewCheckoutHistory();
                    }
                    else if(adminChoice == 1){
                    	cout<<endl;
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
                    else if (adminChoice == 5){
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
                	cout<<endl;
                    string productName;
                    cout<<"Enter product name: ";
                    cin>>productName;
                    Product* product=bst.findProductByName(productName);
                    if(product){
                        cout<<"Product Found: "<<product->name<<", Price: $"<<product->price<<", Quantity: "<<product->quantity<<endl;
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
    delete user;  		// Clean up user object
    return 0;
}
