git init
git clone https://github.com/gitstash/devnull
cd devnull/
git remote add origin https://github.com/gitstash/devnull.git
git remote set-url origin https://github.com/gitstash/devnull.git
git config --global user.email manishamails@gmail.com
git config --global user.name "Manisha Singh"
git commit -m "code"
git remote -v
git push -u origin master

#ssh-keygen
#ssh-copy-id -i ~/.ssh/id_rsa gitstash@github.com

