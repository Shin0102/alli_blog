#!/bin/bash

# 빌드나 푸시가 실패하면 그 시점에서 멈춘다.
# (예전에는 hugo 가 실패해도 그대로 커밋/푸시가 진행됐다)
set -e

echo -e "\033[0;32mDeploying updates to GitHub...\033[0m"

# 커밋 메시지: 인자로 주면 그걸 쓰고, 없으면 날짜
msg="rebuilding site $(date)"
if [ $# -eq 1 ]
  then msg="$1"
fi

# Build the project.
hugo -t zzo

# Go To Public folder
cd public

# Add changes to git.
git add .

# Commit & Push (변경 없으면 건너뜀)
if git diff --cached --quiet; then
  echo "public: 변경 없음 - 건너뜀"
else
  git commit -m "$msg"
  git push origin HEAD:master
fi

# Come Back up to the Project Root
cd ..


# blog 저장소 Commit & Push
git add .

if git diff --cached --quiet; then
  echo "blog: 변경 없음 - 건너뜀"
else
  git commit -m "$msg"
  git push origin master
fi
