python configure.py --amalgamation --disable-shared --cc=gcc --no-autoload --enable-modules=auto_rng,rsa,sha1_sse2,sha1,sha2_32,sha2_64,sha3,emsa1,emsa_pkcs1,emsa_pssr,emsa_raw,dh,ecdh,emsa_x931,dev_random,md5,crc32,cbc,des,blowfish,curve25519,dsa,aes,aes_ssse3,tls --cpu=arm64 --os=darwin --with-local-config=config_ios.h

mv -f botan*.h ../platforms/ios_64b/
mv -f botan*.cpp ../platforms/ios_64b/

python configure.py --amalgamation --disable-shared --cc=gcc --no-autoload --enable-modules=auto_rng,rsa,sha1_sse2,sha1,sha2_32,sha2_64,sha3,emsa1,emsa_pkcs1,emsa_pssr,emsa_raw,dh,ecdh,emsa_x931,dev_random,md5,crc32,cbc,des,blowfish,curve25519,dsa,aes,aes_ssse3,tls --cpu=arm32 --os=darwin --with-local-config=config_ios.h

mv -f botan*.h ../platforms/ios_32b/
mv -f botan*.cpp ../platforms/ios_32b/


