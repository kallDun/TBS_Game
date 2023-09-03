# TBS_Game

### Here it is develop read me

There are few ways to merge to git + svn:
1. If we have updates just on git -> we merge to git (Develop branch)
NOTE: If everything is fine -> merge to master branch
2. If we have changes on svn -> we have to do changes in git and svn (merge both if it is possible - first - svn, second - git)
NOTE: If server is off -> do PR/fork for git changes and wait for server ON
3. If svn and git have changes -> first - update commit svn, second - merge git 