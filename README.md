# K-map-based-logic-minimization
利用Quine-McCluskey algorithm將輸入的k-map資料最小化
## 如何運行程式
編譯後執行，輸入資料檔名

## 資料格式
### 輸入

K-map 格式
```
N
I1 I2 I3 I4 ... 
```
* N : N筆資料
* I : 資料(0,1,2)
  * 0 : 0
  * 1 : 1
  * 2 : X

#### 輸入範例
輸入
```
2
1 0 1 1 0 1 1 0 0 1 1 0 1 1 1 1
2 1 1 2 0 2 1 0 0 0 1 0 0 0 1 0
```
其中的
`1 0 1 1 0 1 1 0 0 1 1 0 1 1 1 1`
轉換成K-map 方形的格式為
```
1 0 1 1
0 1 1 0
1 1 0 1
1 1 1 1
```

### 輸出格式
為boolean function，SOP(sum of product) form
```
#N
I(1,1) I(1,2) I(1,3) I(1,4)
I(2,1) I(2,2) I(2,3) I(2,4)
I(3,1) I(3,2) I(3,3) I(3,4)
I(4,1) I(4,2) I(4,3) I(4,4)
```

* N : 第N筆輸出
* I : 資料(0,1,2)
  * 0 : comlement form
  * 1 : normal form
  * 2 : don't care(does not appear in the term)

#### 輸出範例
輸出
```
#1
2 1 2 1
2 0 2 0
2 2 1 1
1 2 2 1
#2
2 2 1 1
0 0 2 2
```

若輸入為 w,x,y,z

`2 1 2 1` 顯示為 xz
```
#1
xz+x'z'+yz+wz
#2
yz+w'x'
```
