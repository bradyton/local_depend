alias app_link='xdg-open /usr/share/applications/'
alias dict='goldendict'
alias r='Rscript -e' #Note capital 'R' is for running R in bash
alias render_rmdbk='~/bin/render_rmarkdown_book'
alias rmdbk='vim ~/local_depend/GitHubPage/my_R_book/Rmarkdown_features/rmd_features.rmd'
alias new_rmd='~/bin/append_head_rmd'
alias R='R --no-save -q'

export RSTUDIO_PANDOC=/usr/lib/rstudio/bin/pandoc
PS1="\[\033[1;32m\]\u@nb:\[\033[1;34m\]\w\[\033[00m\]$ "
export PS1
export PATH=~/bin:"$PATH"  #Add ~/bin to PATH so custom shell programs can be found

# Define variables for SSH remote pc.
### Reads IP from git repo ~/local_depend/hostname/hostname.txt ###

rm_MAC="44:87:fc:a1:0b:cc" #MAC address of remote pc
rm_ip="`head -1 ~/local_depend/hostname/hostname.txt`" #Now floating IP, needs to change to static
rm_ip_b=`bash ~/ip2ipb.sh $rm_ip` #see 'ip2ipb.sh for detail'
ssh_pc="liao961120@$rm_ip"  #used as 'ssh $ssh_pc'


# Backup ~/bin (Custom shell programs)
cp -r ~/bin/* ~/local_depend/Ubuntu_16.04_setup/sh_program_bak/

# Backup self-defined cmd in git repo (prt lines after 'line 120')
tail -n +120  ~/.bashrc > ~/local_depend/Ubuntu_16.04_setup/mybashrc.sh
alias bak_rc='tail -n +120  ~/.bashrc > ~/local_depend/Ubuntu_16.04_setup/mybashrc_man.sh; echo -e "\n\n\n#### Backup Time\t$(date) ####" >> ~/local_depend/Ubuntu_16.04_setup/mybashrc_man.sh'




#### Backup Time	Thu Aug  2 12:22:37 CST 2018 ####
