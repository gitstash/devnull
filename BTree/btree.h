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
        pthread_mutex_t mLock; // Lock for this tree(and all of the subtree below)

        void printTree(Node *root);
        bool getMin(Node *node, int& minVal);
        bool getMax(Node *node, int& maxVal);
    public:
        Btree():mRoot(NULL), mLock(PTHREAD_MUTEX_INITIALIZER) {}
        ~Btree() {}
        bool insertNode(int data );
        void printTree();
        void updateNode(int oldData, int data);
        void deleteNode(int data);
        void printBFS();
};
