# setup_git_aliases.ps1

# Set Git aliases
git config --global alias.g "!git"
git config --global alias.a "add"
git config --global alias.ss "status"
git config --global alias.co "checkout"
git config --global alias.b "branch"
git config --global alias.l "log"
git config --global alias.ll "log --oneline --graph --decorate"
git config --global alias.c "commit"
git config --global alias.ca "commit --amend"
git config --global alias.sh "show HEAD"
git config --global alias.d "diff"
git config --global alias.ds "diff --staged"
git config --global alias.dc "diff --cached"
git config --global alias.r "reset"
git config --global alias.rh "reset --hard"
git config --global alias.rh1 "reset --hard HEAD~1"

# Set the 'g' alias in PowerShell's $PROFILE to run 'git'
$profilePath = $PROFILE

# Check if the profile file exists; if not, create it
if (-not (Test-Path $profilePath)) {
    New-Item -ItemType File -Force -Path $profilePath
}

# Add the alias to the profile if it's not already there
$aliasScript = "Set-Alias g git"

# Check if the alias is already defined to avoid duplication
if ((Get-Content $profilePath) -notcontains $aliasScript) {
    Add-Content -Path $profilePath -Value "`n$aliasScript"
    Write-Host "'g' alias added to PowerShell profile."
} else {
    Write-Host "'g' alias already exists in PowerShell profile."
}

Write-Host "Git aliases and PowerShell 'g' alias have been set!"
