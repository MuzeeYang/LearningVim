.PHONY=all,clean,update

install:
	@mkdir -p ~/.vim/syntax || echo ''
	@cp -v ./.vim/vimrc ~/.vim
	@cp -v ./.vim/syntax/c.vim ~/.vim/syntax
	@cp -v ./.vim/syntax/common.vim ~/.vim/syntax
	@cp -v ./.myprofile ~/.myprofile
	ctags --fields=+iaS --extra=+q -R -f ~/.vim/systags /usr/include /usr/local/include
	
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
