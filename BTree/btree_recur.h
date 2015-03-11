class Btree
{
    /*create nested class */
    class Node 
    {
        private:
            friend class Btree;
            int mData;
            Node *mLeft;
            Node *mRight;
        public:
            /*constructor with initialization list */
            Node(int data):mData(data),
                           mLeft(NULL),
                           mRight(NULL) {}
            ~Node() {}
    };
    private:
        Node *mRoot;

        void printTree(Node *root);
        bool getMin(Node *node, int& minVal);
        bool getMax(Node *node, int& maxVal);
        Node *insertNode(Node *root, int data);
    public:
        Btree():mRoot(NULL)  {}
        ~Btree() {}
        void insertNode(int data );
        void printTree();
        Node *updateNode(int oldData, int data);
        Node *deleteNode(int data);
};
