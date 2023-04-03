#include <iostream>

using namespace std;

struct node
{
    int size;
    int key[3];
    node* first;
    node* second;
    node* third;
    node* fourth;
    node* parent;
};

struct level
{
    node* aim;
    level* next;
    int level;
};

void swap(int& x, int& y)
{
    int t;
    t = x;
    x = y;
    y = t;
}

void sort(node*& ptr)
{
    if (ptr->size == 1)
        return;
    if ((ptr->size == 2) && (ptr->key[0] > ptr->key[1]))
    {
        int x;
        x = ptr->key[0];
        ptr->key[0] = ptr->key[1];
        ptr->key[1] = x;
    };
    if (ptr->size == 3)
    {
        if (ptr->key[0] > ptr->key[1]) swap(ptr->key[0], ptr->key[1]);
        if (ptr->key[0] > ptr->key[2]) swap(ptr->key[0], ptr->key[2]);
        if (ptr->key[1] > ptr->key[2]) swap(ptr->key[1], ptr->key[2]);
    };
};

void insert_to_node(node*& ptr, int k)
{
    ptr->key[ptr->size] = k;
    ptr->size++;
    sort(ptr);
};

node* split(node* item)
{
    if (item->size < 3) return item;
    node* x = new node;
    x->key[0] = item->key[0];
    x->first = item->first;
    x->second = item->second;
    x->third = nullptr;
    x->fourth = nullptr;
    x->parent = item->parent;
    x->size = 1;
    node* y = new node;
    y->key[0] = item->key[2];
    y->first = item->third;
    y->second = item->fourth;
    y->third = nullptr;
    y->fourth = nullptr;
    y->parent = item->parent;
    y->size = 1;
    if (x->first != nullptr)  x->first->parent = x;
    if (x->second != nullptr) x->second->parent = x;
    if (y->first != nullptr)  y->first->parent = y;
    if (y->second != nullptr)  y->second->parent = y;

    if (item->parent != nullptr)
    {
        insert_to_node(item->parent, item->key[1]);
        if (item->parent->first == item) item->parent->first = nullptr;
        else if (item->parent->second == item) item->parent->second = nullptr;
        else if (item->parent->third == item) item->parent->third = nullptr;

        if (item->parent->first == nullptr) {
            item->parent->fourth = item->parent->third;
            item->parent->third = item->parent->second;
            item->parent->second = y;
            item->parent->first = x;
        }
        else if (item->parent->second == nullptr) {
            item->parent->fourth = item->parent->third;
            item->parent->third = y;
            item->parent->second = x;
        }
        else {
            item->parent->fourth = y;
            item->parent->third = x;
        }
        node* tmp = item->parent;
        delete item;
        return tmp;
    }
    else
    {
        x->parent = item;
        y->parent = item;
        item->first = x;
        item->second = y;
        item->third = nullptr;
        item->fourth = nullptr;
        item->parent = nullptr;
        item->key[0] = item->key[1];
        item->size = 1;
        return item;
    }
};

node* insert(node*& ptr, int k)
{
    if (ptr == nullptr)
    {
        ptr = new node;
        ptr->key[0] = k;
        ptr->size = 1;
        ptr->first = nullptr;
        ptr->second = nullptr;
        ptr->third = nullptr;
        ptr->fourth = nullptr;
        ptr->parent = nullptr;
    }
    else if ((ptr->first == nullptr) && (ptr->second == nullptr) && (ptr->third == nullptr))
    {
        insert_to_node(ptr, k);
    }
    else if (k <= ptr->key[0]) insert(ptr->first, k);
    else if ((ptr->size == 1) && k >= ptr->key[0] || (ptr->size == 2 && k < ptr->key[1])) insert(ptr->second, k);
    else insert(ptr->third, k);
    return split(ptr);
};

void output(level*& p)
{
    if (p == nullptr) return;
    level* temp, * back;
    temp = back = p;
    temp->next = nullptr;
    while (temp != nullptr)
    {
        if (temp->aim != nullptr && temp->aim->first != nullptr)
        {
            back->next = new level;
            back->next->aim = temp->aim->first;
            back = back->next;
            back->level = temp->level + 1;
            back->next = nullptr;
        }
        if (temp->aim != nullptr && temp->aim->second != nullptr)
        {
            back->next = new level;
            back = back->next;
            back->level = temp->level + 1;
            back->aim = temp->aim->second;
            back->next = nullptr;
        }
        if (temp->aim != nullptr && temp->aim->third != nullptr)
        {
            back->next = new level;
            back = back->next;
            back->level = temp->level + 1;
            back->aim = temp->aim->third;
            back->next = nullptr;
        }
        temp = temp->next;
    }
    temp = p;
    int l = 1;
    cout << "Level " << l << ": ";
    while (temp != nullptr)
    {
        if (temp->level != l)
        {
            l++;
            cout << endl << "Level " << l << ": ";
        }
        int i;
        if (temp->aim != nullptr)
        {
            for (i = 0; i < temp->aim->size; i++)
            {
                cout << temp->aim->key[i] << " ";
            }
            if (temp->aim->parent != nullptr)
            {
                cout << "( ";
                for (i = 0; i < temp->aim->parent->size; i++)
                {
                    cout << temp->aim->parent->key[i] << " ";
                }
                cout << ") ";
            }
        }
        cout << "  ";
        temp = temp->next;
    }
    cout << endl;
    temp = p;
    level* temp2;
    temp2 = p;
    do
    {
        temp2 = temp;
        temp = temp->next;
        delete temp2;
    } while (temp != nullptr);
};

void deleteTree(node* p)
{
    if (p != nullptr)
    {
        deleteTree(p->first);
        deleteTree(p->second);
        deleteTree(p->third);
        delete p;
    }
};

int main()
{
    int x;
    node* Tree = nullptr;
    level* out = nullptr;

    cout << "Enter new element of tree: " << endl;
    cin >> x;
    cout << endl;
    while (x != 0)
    {
        insert(Tree, x);
        out = new level;
        out->aim = Tree;
        out->level = 1;
        output(out);
        cout << endl;
        cout << "\n";
        cout << "Enter new element of tree or 0 to end tree's building: " << endl;
        cin >> x;
        cout << endl;
    };
    deleteTree(Tree);
}