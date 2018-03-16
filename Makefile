.PHONY=all,clean,update

all:
	cp -ir ./.vim ~
	cp ./myprofile ~/.myprofile
	cp -rf ./shl ~
	if [ -f ~/.bashrc ];then cat ./proCache >> ~/.bashrc;fi
	if [ -f ~/.profile ];then cat ./proCache >> ~/.profile;fi
	ctags --fields=+iaS --extra=+q -R -f ~/.vim/systags /usr/include /usr/local/include
	
update:
	cp -ir ~/.vim .
	cp ~/.myprofile ./myprofile
	cp -rf ~/shl .
	ctags --fields=+iaS --extra=+q -R -f ~/.vim/systags /usr/include /usr/local/include

clean:
	rm -ir ~/.vim
	rm ~/.myprofile
	rm -rf ~/shl
	@echo "remove the context under manually."
	@cat ./proCache
