#include <cstdio>
#include <iostream>

#include <algorithm>
using namespace std;

typedef long long LL;

const int maxn = 1e5 + 10;

struct Point {
    int coordinate[2], l, r, id; //0根据x轴上的数值切割 1根据y轴上的数值切割
    void input(int i){
        id = i;
        scanf("%d%d", &coordinate[0], &coordinate[1]);
    }
} poolPoint[maxn];
int idx[maxn];
int boolWeight;
LL ans;

bool cmp(const Point& left, const Point& right){
    return left.coordinate[boolWeight] < right.coordinate[boolWeight];
}

int build(int l, int r, int weight){
    int mid = (l + r) >> 1; boolWeight = weight;
    nth_element(poolPoint + l, poolPoint + mid, poolPoint + 1 + r, cmp);
    idx[poolPoint[mid].id] = mid;
    
    poolPoint[mid].l = l != mid ? build(l, mid - 1, !weight) : 0;
    poolPoint[mid].r = r != mid ? build(mid + 1, r, !weight) : 0;
    return mid;
}

LL dist(LL x, LL y = 0){
    return x * x + y * y;
}

void query(int kdid, int weight, LL x, LL y){
    LL temp = dist(x - poolPoint[kdid].coordinate[0], 
                    y - poolPoint[kdid].coordinate[1]);
    if(temp) ans = min(ans, temp);
    
    if(poolPoint[kdid].l && poolPoint[kdid].r){
        //选定要留下的切面
        bool flag = !weight ? (x <= poolPoint[kdid].coordinate[0]) :
                            (y <= poolPoint[kdid].coordinate[1]);
        query(flag ? poolPoint[kdid].l : poolPoint[kdid].r, !weight, x, y);
        
        //判断被切去得切面需要另外判断不
        LL d = !weight ? dist(x - poolPoint[kdid].coordinate[0]) : 
                    dist(y - poolPoint[kdid].coordinate[1]);
        if(d < ans) query(flag ? poolPoint[kdid].r : poolPoint[kdid].l, !weight, x, y);
    }
    else if(poolPoint[kdid].l) query(poolPoint[kdid].l, !weight, x, y);
    else if(poolPoint[kdid].r) query(poolPoint[kdid].r, !weight, x, y);
}

int main(int argc, char **argv)
{
    int T, n;
    scanf("%d", &T);
    for(int i = 0; i < T; i++){
        scanf("%d", &n);
        for(int i = 1; i <= n; i++) poolPoint[i].input(i);
        int kdtreeRoot = build(1, n, 0);
        for(int i = 1; i <= n; i++){
            ans = 1e18;
            query(kdtreeRoot, 0, poolPoint[idx[i]].coordinate[0], 
                                poolPoint[idx[i]].coordinate[1]);
            printf("%I64d\n", ans);
        }
    }
	return 0;
}
