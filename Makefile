.PHONY=all,clean,update

install:
	@mkdir -p ~/.vim/syntax || echo ''
	@cp -vr .vim ~
	@cp -v ./.myprofile ~/.myprofile
	#ctags --fields=+iaS --extra=+q -R -f ~/.vim/systags /usr/include /usr/local/include
	
clean:
	@rm ~/.vim/vimrc
	@rm ~/.vim/syntax/c.vim
	@rm ~/.vim/syntax/common.vim
	@rm ~/.vim/systags
	@rm ~/.myprofile
	@cat ./proCache
	@echo "remove the context under manually in [~/.bashrc] or [~/.profile]."

update:
	@cp -vr ~/.vim .
	@cp -v ~/.myprofile .
	git add .
	git commit -m"update `date`"
