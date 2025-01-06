echo "$PRE_COMMIT_FILE"


GIT="./.git"

if [ -e "$GIT" ]; then
    if [ -f "$GIT" ]; then
	PRE_COMMIT_FILE=`cat $GIT | awk 'NR==1 {print $2; exit}'`/hooks/pre-commit
    elif [ -d "$GIT" ]; then
	PRE_COMMIT_FILE=./.git/hooks/pre-commit
    fi
else
    echo "not in a repository"
fi
cp ./tools/pre-commit $PRE_COMMIT_FILE
