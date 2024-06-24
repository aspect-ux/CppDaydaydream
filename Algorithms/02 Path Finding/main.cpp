#include <iostream>
#include <math.h>
#include <queue>
#include <vector>
#include <unordered_set>

std::vector<int> start = { 2, 8, 3,1, 6, 4,7, 0, 5 };
int final[9] = { 1, 2, 3,8, 0, 4, 7, 6, 5 };
int HType = -1;
int dir[4][2] = { {1,0},{0,1},{-1,0},{0,-1} };//下右上左

// 优先队列排序规则
//struct cmp1 {
//	bool operator()(const node r, const node l)
//	{
//		return r.h < l.h;
//	}
//};

enum DIR {
    DOWN, RIGHT, UP, LEFT
};
int countH(std::vector<int> status, int HType);
struct Node
{
    std::vector<int> status; // 八数码
    int G; // 当前第几步
    int H; // 不在位置的个数或曼哈顿距离和
    int F; // 总耗散值
    int Zero; // 0的位置
    int step; // 方向参考数组dir，0-3
    Node* Parent;
    int childCount;

    Node()
    {
        status = start;
        G = 0;
        H = countH(status, HType);
        F = H + G;
        step = -1;
        Zero = 7;
        Parent = NULL;
        childCount = 0;
    }
};

// 定义OPEN表和CLOSE表
std::vector<Node*> OPEN;
std::vector<Node*> CLOSE;

std::vector<Node*> ans;
// 判断是否出界
bool InBound(int x, int y)
{
    return x >= 0 && x <= 2 && y >= 0 && y <= 2;
}
bool isOver(std::vector<int> status)
{
    for (int i = 0; i < 9; i++) {
        if (final[i] != status[i])
            return false;
    }
    return true;
}
void swap(int& a, int& b)
{
    int tmp = a;
    a = b;
    b = tmp;
}
// 计算当前启发值
int countH(std::vector<int> status, int h)
{
    int cnt = 0;
    switch (h)
    {
    case 0:	// H(x) = 0
        break;
    case 1: // H(x) = 错位数
        for (int i = 0; i < 9; i++)
        {
            if (final[i] != status[i])
                cnt++;
        }
        break;
    case 2: // H(x) = 错位曼哈顿距离和
        for (int i = 0; i < 9; i++)
        {
            for (int j = i; j < 9; j++)
            {
                if (final[i] == status[j])
                {
                    int a = i / 3, b = i % 3, x = j / 3, y = j % 3;
                    cnt += abs(a - x) + abs(b - y);
                    break;
                }
            }
        }
        break;
    default:
        std::cout << "INPUT FOR H(X) IS WRONG" << std::endl;
        cnt = -1;
        break;
    }
    return cnt;
}
void initRoot(Node*& root)
{

}

void printStepData(int opt, int travel)
{

    printf("h=%d-----\n", HType);
    printf("opt step is %d\n", opt);
    printf("travel step is %d\n", travel);
    float efficiency = ((float)opt) * 100.0f / travel;
    printf("efficiency is %.2f%%\n", efficiency);

    if (HType == 2)
    {
        int t = opt;
        for (auto c : ans)
        {
            std::cout << "Step: " << (t++) - opt + 1 << std::endl;
            switch (c->step)
            {
            case 0:
                std::cout << "Down" << "->" << std::endl;
                break;
            case 1:
                std::cout << "RIGHT" << "->" << std::endl;
                break;
            case 2:
                std::cout << "UP" << "->" << std::endl;
                break;
            case 3:
                std::cout << "LEFT" << "->" << std::endl;
                break;
            default:
                std::cout << "->" << std::endl;
                break;
            }

            std::cout << "[";
            for (int p = 0; p < 3; p++)
            {
                if (p == 2)
                    std::cout << "[" << c->status[p * 3] << ' ' << c->status[p * 3 + 1] << ' ' << c->status[p * 3 + 2] << "]" << "]" << std::endl;
                else
                    std::cout << "[" << c->status[p * 3] << ' ' << c->status[p * 3 + 1] << ' ' << c->status[p * 3 + 2] << "]" << std::endl;
            }
        }
    }
}
bool Exist(Node* tmp)
{
    for (auto c : CLOSE)
    {
        if (c->status == tmp->status)
        {
            //printf("-----%d=%d  有%d----\n", c.Zero, tmp->Zero,n);
            return true;
        }
    }
    return false;
}

void createNextNode(Node*& tmp, int a, int b, int k, Node*& e)
{
    tmp->status = e->status;
    tmp->Zero = e->Zero;
    tmp->G = e->G + 1;
    swap(tmp->status[tmp->Zero], tmp->status[a * 3 + b]);
    tmp->H = countH(tmp->status, HType);
    tmp->F = tmp->G + tmp->H;
    tmp->Zero = a * 3 + b;
    tmp->step = k;
    tmp->Parent = e;
    e->childCount++;
}

int main()
{

    struct VectorHash {
        size_t operator()(const std::vector<int>& v) const {
            std::hash<int> hasher;
            size_t seed = 0;
            for (int i : v) {
                seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
    using MySet = std::unordered_set<std::vector<int>, VectorHash>;
    //MySet ans;
    //std::unordered_set<std::vector<int>> ans;
    while (1)
    {
        std::cout << "PLEASE INPUT H(X):";
        std::cin >> HType;
        int opt = 0, travel = 0, travel2 = 0, travel3 = 0, flag = 0;
        float efficiency = 0.0f;
        std::queue<Node*> qu;
        Node* root = new Node();
        //initRoot(root); //初始化
        qu.push(root);
        OPEN.resize(0);
        ans.resize(0);
        // HType = 0,广搜；HTpe = 1,2 启发式搜索
        while (!qu.empty() || flag)
        {
            opt++;//深度加一
            int n = qu.size();
            //std::vector<Node> CLOSE_Tmp;
            for (int i = 0; i < n; i++) {
                Node* e = qu.front();
                qu.pop();
                int x = e->Zero / 3, y = e->Zero % 3;
                //当前节点的四个方向判断
                for (int k = 0; k < 4; k++) {
                    int a = x + dir[k][0], b = y + dir[k][1];
                    if (InBound(a, b))
                    {
                        Node* tmp = new Node();
                        //创建下一个节点tmp
                        createNextNode(tmp, a, b, k, e);

                        if (isOver(tmp->status)) {
                            if (HType == 0)
                                printStepData(opt + 1, travel);
                            while (tmp->Parent)
                            {
                                ans.insert(ans.begin(), tmp);
                                tmp = tmp->Parent;
                            }
                            ans.insert(ans.begin(), tmp);
                            //printStepData(opt+1, travel+OPEN.size());
                            flag = 1;
                        }

                        if (e->Parent && tmp->status != e->Parent->status || !e->Parent)
                        {
                            travel++;
                            OPEN.push_back(tmp);
                        }
                    }
                }
                //CLOSE_tmp->push_back(e);
            }
            //std::cout << n << std::endl;
            /*
            //维护CLOSE数组(每一层的非叶子）
            //CLOSE = CLOSE_Tmp;*/

            //维护OPEN数组(当前进度的叶子，以后可能不是）
            //比较H的值来判断哪些是需要计算的节点,注意返回的是迭代器，需要加才等价于Node
            /*auto min_open = min_element(OPEN.begin(), OPEN.end(), [&](Node*& a, Node*& b)
                {
                    return a->F < b->F;
                });*/

            int q = OPEN.size();
            if (q == 0) { std::cout << "无解"; int b = 0; std::cin >> b; }
            sort(OPEN.begin(), OPEN.end(), [](Node*& a, Node*& b)
                {
                    return a->F < b->F;
                });
            for (auto c : OPEN)
            {
                if (c->F == (OPEN[0])->F)
                {
                    //找到用的节点
                    qu.push(c);
                    travel2++;
                    travel3++;
                    //CLOSE.push_back(OPEN[k]);
                }
            }
            if (flag)
            {

            }
            //CLOSE.insert(OPEN.begin(), OPEN.end());
            //清理OPEN
            OPEN.clear();//没有释放指针
            //std::cout << qu.size() << std::endl;
            if (flag) break;
        }
        if (HType == 1)
            printStepData(opt + 1, travel2 + 1);
        else if (HType == 2)
            printStepData(opt + 1, travel3 - 3);
    }
    system("pause:");
    return 0;
}