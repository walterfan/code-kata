#include "kata_helper.h"
#include <queue>


// Function to add an element to the stack
bool Stack::push(int x) {
    if (isFull()) {
        std::cout << "Stack Overflow\n";
        return false;
    } else {
        arr[++top] = x;
        std::cout << x << " pushed into stack\n";
        return true;
    }
}

// Function to remove an element from the stack
int Stack::pop() {
    if (isEmpty()) {
        std::cout << "Stack Underflow\n";
        return 0;
    } else {
        return arr[top--];
    }
}

// Function to return the top element of the stack
int Stack::peek() {
    if (isEmpty()) {
        std::cout << "Stack is Empty\n";
        return 0;
    } else {
        return arr[top];
    }
}

// Function to check if the stack is empty
bool Stack::isEmpty() {
    return top == -1;
}

// Function to check if the stack is full
bool Stack::isFull() {
    return top == capacity - 1;
}


// Function to add an element to the queue
bool Queue::enqueue(int x) {
    if (isFull()) {
        std::cout << "Queue Overflow\n";
        return false;
    } else {
        back = (back + 1) % capacity;
        arr[back] = x;
        size++;
        std::cout << x << " enqueued into queue\n";
        return true;
    }
}

// Function to remove an element from the queue
int Queue::dequeue() {
    if (isEmpty()) {
        std::cout << "Queue Underflow\n";
        return 0;
    } else {
        int item = arr[front];
        front = (front + 1) % capacity;
        size--;
        return item;
    }
}

// Function to return the front element of the queue
int Queue::peekFront() {
    if (isEmpty()) {
        std::cout << "Queue is Empty\n";
        return 0;
    } else {
        return arr[front];
    }
}

// Function to return the back element of the queue
int Queue::peekBack() {
    if (isEmpty()) {
        std::cout << "Queue is Empty\n";
        return 0;
    } else {
        return arr[back];
    }
}

// Function to check if the queue is empty
bool Queue::isEmpty() {
    return (size == 0);
}

// Function to check if the queue is full
bool Queue::isFull() {
    return (size == capacity);
}

// ---------------- Binary Tree ----------------
void BinaryTree::insert(int data) {
    TreeNode* newNode = new TreeNode(data);
    if (root == nullptr) {
        root = newNode;
        return;
    }

    TreeNode* current = root;
    TreeNode* parent = nullptr;

    while (true) {
        parent = current;
        if (data < current->val) {
            current = current->left;
            if (current == nullptr) {
                parent->left = newNode;
                return;
            }
        } else {
            current = current->right;
            if (current == nullptr) {
                parent->right = newNode;
                return;
            }
        }
    }
}

// Function for inorder traversal (Left, Root, Right)
void BinaryTree::inorderTraversal(TreeNode* node) {
    if (node == nullptr)
        return;

    inorderTraversal(node->left);
    std::cout << node->val << " ";
    inorderTraversal(node->right);
}

// Function for preorder traversal (Root, Left, Right)
void BinaryTree::preorderTraversal(TreeNode* node) {
    if (node == nullptr)
        return;

    std::cout << node->val << " ";
    preorderTraversal(node->left);
    preorderTraversal(node->right);
}

// Function for postorder traversal (Left, Right, Root)
void BinaryTree::postorderTraversal(TreeNode* node) {
    if (node == nullptr)
        return;

    postorderTraversal(node->left);
    postorderTraversal(node->right);
    std::cout << node->val << " ";
}

// Function to add an edge to the graph
void Graph::addEdge(int v, int w) {
    adj[v].push_back(w); // Add w to v’s list.
    adj[w].push_back(v); // Add v to w’s list (because the graph is undirected).
}

// Function to perform DFS traversal from a given vertex
void Graph::DFS(int v) {
    std::vector<bool> visited(V, false); // Mark all the vertices as not visited
    std::list<int> stack;

    // Push the current source node
    stack.push_back(v);

    while (!stack.empty()) {
        // Pop a vertex from stack
        v = stack.back();
        stack.pop_back();

        // Stack may contain the same vertex twice, so we need to check if it's already visited
        if (!visited[v]) {
            std::cout << v << " ";
            visited[v] = true;
        }

        // Get all adjacent vertices of the popped vertex v
        // If an adjacent has not been visited, push it to the stack
        for (auto i = adj[v].begin(); i != adj[v].end(); ++i) {
            if (!visited[*i]) {
                stack.push_back(*i);
            }
        }
    }
}

// Function to perform BFS traversal from a given vertex
void Graph::BFS(int v) {
    std::vector<bool> visited(V, false); // Mark all the vertices as not visited
    std::list<int> queue;

    // Mark the current node as visited and enqueue it
    visited[v] = true;
    queue.push_back(v);

    while (!queue.empty()) {
        // Dequeue a vertex from queue
        v = queue.front();
        std::cout << v << " ";
        queue.pop_front();

        // Get all adjacent vertices of the dequeued vertex v
        // If an adjacent has not been visited, then mark it visited and enqueue it
        for (auto i = adj[v].begin(); i != adj[v].end(); ++i) {
            if (!visited[*i]) {
                visited[*i] = true;
                queue.push_back(*i);
            }
        }
    }
}



void print_tree(TreeNode* root, std::ostream& out, TraversalOrder order) {
    if (root == nullptr) {
        std::cout << "null" << std::endl;
        return;
    }
    if (order == TraversalOrder::PRE_ORDER) {
        out << root->val << " ";
        print_tree(root->left, out, order);
        print_tree(root->right, out, order);
    } else if (order == TraversalOrder::IN_ORDER) {
        print_tree(root->left, out, order);
        out << root->val << " ";
        print_tree(root->right, out, order);
    } else if (order == TraversalOrder::POST_ORDER) {
        print_tree(root->left, out, order);
        print_tree(root->right, out, order);
        out << root->val << " ";
    } else if (order == TraversalOrder::LEVEL_ORDER) {
        std::queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();
            out << node->val << " ";
            if (node->left!= nullptr) {
                q.push(node->left);
            }
            if (node->right!= nullptr) {
                q.push(node->right);
            }
        }
    }

}


void print_array(const std::vector<int>& nums, std::ostream& out ) {
    for (auto num : nums) {
        out << num << " ";
    }
    out << std::endl;
}

void print_list(ListNode* head, std::ostream& out) {
    while (head!= nullptr) {
        out << head->val << " ";
        head = head->next;
    }
    out << std::endl;
}


// Function to merge two halves of the array
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Temporary arrays to hold the two halves
    std::vector<int> leftArr(n1);
    std::vector<int> rightArr(n2);

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr
    int i = 0; // Initial index of the first subarray
    int j = 0; // Initial index of the second subarray
    int k = left; // Initial index of the merged subarray

    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftArr, if any
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightArr, if any
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

// Function to perform merge sort on the array
void merge_sort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort the first and second halves
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

// Function to partition the array into two sub-arrays
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // Choose the last element as the pivot
    int i = low - 1; // Index of the smaller element

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i; // Increment index of smaller element
            std::swap(arr[i], arr[j]); // Swap elements
        }
    }

    // Swap the pivot element with the element at index i + 1
    std::swap(arr[i + 1], arr[high]);
    return i + 1; // Return the partitioning index
}

// Function to perform quick sort on the array
void quick_sort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high); // Partitioning index

        // Recursively sort elements before and after partition
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

void insert_sort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;

        // Move elements of arr[0..i-1] that are greater than key to one position ahead of their current position
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

int binary_search(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2; // Calculate the middle index

        // Check if the target is present at mid
        if (arr[mid] == target) {
            return mid; // Target found at index mid
        }

        // If target is greater, ignore the left half
        if (arr[mid] < target) {
            left = mid + 1;
        }
        // If target is smaller, ignore the right half
        else {
            right = mid - 1;
        }
    }

    return -1; // Target not found
}


ListNode* build_list(std::vector<int>& nums) {
    ListNode* head = new ListNode(nums[0]);
    ListNode* cur = head;
    for (int i = 1; i < nums.size(); ++i) {
        cur->next = new ListNode(nums[i]);
        cur = cur->next;
    }
    return head;
}