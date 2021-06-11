.PHONY=all,clean,update

install:
	@mkdir -p ~/.vim/syntax || echo ''
	@cp -vr .vim ~
	@cp -v ./.myprofile ~/.myprofile
	
clean:
	@rm ~/.vim/vimrc
	@rm ~/.myprofile
	@echo "remove the context under manually in [~/.bashrc] or [~/.profile]."

update:
	@cp -vr ~/.vim .
	@cp -v ~/.myprofile .
	git add .
	git commit -m"update `date`"
