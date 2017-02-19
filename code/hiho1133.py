# 描述

# 在上一回里我们知道Nettle在玩《艦これ》，Nettle的镇守府有很多船位，但船位再多也是有限的。Nettle通过捞船又出了一艘稀有的船，但是已有的N(1≤N≤1,000,000)个船位都已经有船了。所以Nettle不得不把其中一艘船拆掉来让位给新的船。Nettle思考了很久，决定随机选择一个k，然后拆掉稀有度第k小的船。 已知每一艘船都有自己的稀有度，Nettle现在把所有船的稀有度值告诉你，希望你能帮他找出目标船。
# 提示：非有序数组的二分查找之二

# 输入

# 第1行：2个整数N,k。N表示数组长度，
# 第2行：N个整数，表示a[1..N]，保证不会出现重复的数，1≤a[i]≤2,000,000,000。

# 输出

# 第1行：一个整数t，表示t在数组中是第k小的数，若K不在数组中，输出-1。

def findK (a, low, high, k):
    first = low
    last = high
    key = a[first]
    while first < last:
        while first < last and a[last] >= key:
            last -= 1
        a[first] = a[last]
        while first < last and a[first] <= key:
            first += 1
        a[last] = a[first]
    a[first] = key
    if first < k:
        return findK(a, first + 1, high, k)
    elif first > k:
        return findK(a, low, first - 1, k)
    else:
        return key
    
while True:
    try: 
        (n, k) = (int(x) for x in raw_input().split())
        a = list((int(x) for x in raw_input().split()))
        if n < k:
            print(-1)
        else:
            print(findK(a, 0, len(a) - 1, k - 1))
    except EOFError:
        break