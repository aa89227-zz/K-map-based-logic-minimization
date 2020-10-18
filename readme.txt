如何運行程式:
	測資名稱為 Case1
	< 輸入 >:
	
	其中
	0 代表 0 
	1 代表 1
	2 代表 X (don’t care) 
	
	example:
	2 					
	1 0 1 1 0 1 1 0 0 1 1 0 1 1 1 1		
	2 1 1 2 0 2 1 0 0 0 1 0 0 0 1 0		<---kmap 格式

	代表有2筆測資
	
	第一筆為
	1011
	0110
	0110
	1111
	
	第二筆為
	2112
	0210
	0010
	0010
	
	< 輸出 >:
	
	#1		<--- 第幾筆測資的結果
	2020	<---	0代表complement form  
	2012		1代表normal form 
	2121		2代表 don’t care (does not appear in the term) 
	1022		#1(1)的結果表示(以wxyz的方式) : x'z' + x'y + xz + wx'
	#2
	0221
	2211


(class):
	binarycode 	//大小為 size 的 binarycode
		
	graycode 	//運用網址的方法將大小為 size 的 graycode 排序完畢 https://openhome.cc/Gossip/AlgorithmGossip/GrayCode.htm
				//結果放入 graycode::list 中 
				//結果為 graycode 在 binarycode 中的 index
				
	McCluskey	//https://zh.wikipedia.org/wiki/%E5%A5%8E%E5%9B%A0%EF%BC%8D%E9%BA%A6%E5%85%8B%E6%8B%89%E6%96%AF%E5%9F%BA%E7%AE%97%E6%B3%95
				//運用網址上的方法
				//先將參數 sop 分為 m() 跟 d()
				//先找到素蘊含項 *findePrime()
				*findePrime 為 recursive function 每一次將大小為 index + 1 的蘊含項找到，結果放入 fullImplicantList[index]中
							使用 *compare() 找到相差為 1 的 implicant (順便去除重複值)
							
							從 fullImplicantList 找出素蘊含項放入prime
				
				//用 Petrick's method 找出最小 *doPetrick()
				*doPetrick	先建立 table --> step 2 of quine-mccluskey
							找出所有可能的組合 *findPossible() 利用set的性質
							找出最小的組合放入minPossibleList