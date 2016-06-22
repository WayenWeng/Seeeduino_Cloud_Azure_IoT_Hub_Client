
#!/bin/ash

cp /mnt/$1/run.log /root

cp /mnt/$1/AzureConnectionString /root

cp /mnt/$1/AzureMessageReceive /root

cp /mnt/$1/AzureMessageSend /root

cp /mnt/$1/AzureIoTHubClient /root

cp /mnt/$1/rc.local /etc/rc.local

cp /mnt/$1/ca-certificates_20160104_all.ipk /root

cd /root
opkg install ca-certificates_20160104_all.ipk

rm /root/ca-certificates_20160104_all.ipk
