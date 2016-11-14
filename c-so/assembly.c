struct text {
        int a;
    };

    struct text function(int n)
    {    
        struct text s;
        s.a = n;
        return s;
    }
    int main(void)
    {
        struct text t = function(10);
        return 0;
    }
