#*.cから*.oは書かなくていいらしい。
#	先ほどの Makefileでは，main.cと sub.cから， それぞれ main.o と sub.oを作成する方法を書いていませんでした． 
#	通常 *.cから *.oの作成手順は省略します． しかし，最適化オプション -Oや， インクルードファイルのありかを明示する -I オプションは *.cから *.oの作成時に指定すべきです． 
#	そのために使われるのが，CFLAGSです． *.cから *.oが自動的に作成される際に，CFLAGSがオプションとして使われます．
#	(http://omilab.naist.jp/~mukaigawa/misc/Makefile.html)
#とのこと。

OBJS=ltest.o
SRCS=$(OBJS:%.o=%.c)
CFLAGS=-g -Wall
LDLIBS=
TARGET=ltest
$(TARGET):$(OBJS)
	gcc $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)
