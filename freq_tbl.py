# coding: utf-8
import sys

freq_lst = []

# 計算
for i in range(28):
	freq = 51.0 * (2 ** (i / 12.0)) # 51 ... 1オクターブ目"ド"。
	freq = round(freq)				# 四捨五入
	freq_lst.append(freq)			# 挿入

# 表示
for i in range(len(freq_lst)):
	# freq_lst[i]
	sys.stdout.write(str(int(freq_lst[i])) + ", ")
	# 4つ出力後、改行
	if (i & 3) == 3:
		print 