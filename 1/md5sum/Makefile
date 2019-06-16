# 1.pdf が正しく生成されたかを確認するにはこのディレクトリに1.pdfをコピーし、
# $ make check
# を実行する。

.PHONY: check

check: 1.pdf
	md5sum -c md5.txt

1.pdf:
	touch $@
