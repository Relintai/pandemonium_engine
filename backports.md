
# Some notes on how to do backports

I'm currently figuring out how to do this properly, these are just notes, so I don't have to find everything again.

https://stackoverflow.com/questions/40757246/git-status-and-git-diff-empty-after-failed-git-am

git remote add patch-origin <path>
git fetch patch-origin



git format-patch  -1


git am --ignore-whitespace --ignore-space-change -3 < 

git am --continue

git mergetool


