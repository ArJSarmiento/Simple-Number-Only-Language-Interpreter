# CMSC-124-Project-SNOL
Simple Number-Only Language (SNOL). SNOL is a simplified custom language that only involves integer and real values, operations, and expressions.

## Cloning branch
```shell
git clone https://github.com/ArJSarmiento/CMSC-124-Project-SNOL.git
```

## Before making changes in dev-branch
```shell
git checkout dev-branch # go to dev-branch
git pull origin main # merges main branch with current branch
git push # push changes to the cloud 
```

## Make changes

## After making changes in dev-branch
### Commit and push changes to dev-branch with a commit message either in the vscode git GUI or with the commands...
```shell
git add .
git commit -m "your commit message"
git push
```
### Then...
```shell
git pull origin main  # merges main branch with current branch
git push  # push changes to the cloud 
git checkout main   # change branch to main
git pull  # get recent changes 
git merge dev-branch  # merge changes from dev-branch
git push # push changes to the cloud 
git checkout dev-branch # return to dev-branch
```

### Run on Linux
```shell
g++ *.cpp -o snol
./snol
```


### Run on Windows
```shell
gcc *.c -o snol
snol.exe
```
