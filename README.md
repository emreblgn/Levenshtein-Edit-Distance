# Levenshtein Edit Distance
 Girilen cümledeki hatalı kelimelere uygun doğru kelime öneren program.

**Program Özellikleri**

1- Sözlük dosyasını okuyup hash tablosuna ekleme.

2- Hatalı girilen kelimelerin tekrar hatalı girilmesi durumu için ayrıca bir hatalı kelime tablosu tutup bu kelimelerin doğrusunun bir sonraki girişlerde direkt olarak bu tablodan alınması.

3- Girilen cümledeki bütün kelimeleri hash tablosunda arama.

 - Kelime hash tablosunda varsa doğru bir kelime olacağından bir sonraki kelimeye geçme.
 
 - Kelime hash tablosunda yoksa hatalı kelime tablosunda arama ve hatalı kelime tablosunda varsa doğrusunu bu tablodan önerme.
 
 - Kelime hatalı kelime tablosundada yoksa sözlük tablosundaki bütün kelimelerle arasındaki mesafenin Levenshtein Edit Distance yöntemiyle ölçülüp varsa mesafesi 1 olan sözcüklerin, yoksa mesafesi 2 olan kelimelerin kullanıcıya önerilmesi, daha büyük mesafeli kelimelerin önerilmemesi.
 
4- Kullanıcı mesafesi verilen kelimelerden birini kabul ederse bu kelimenin ve kullanıcının girdiği hatalı kelimenin hatalı kelime tablosuna eklenmesi.
 
**Bonus**

Hash tablosunda sorgulanan satırdaki en küçük değer 3 ise alt satırlara gidebilecek en küçük değer 3 olacağından ve program sadece 1 ya da 2 uzaklığındaki kelimeleri önereceğinden diğer satırların kontrol edilmesine gerek kalmadan geçerli kelimenin mesafe kontrolünden çıkılabilir.

| | |g|o|o|g|l|e|
| --- | --- | --- | --- | --- | --- | --- | --- |
| |0|1|2|3|4|5|6|
|y|1|1|2|3|4|5|6|
|a|2|2|2|3|4|5|6|
|h|3|3|3|3|4|5|6| 
|o|4|4|3|3|4|5|6|
|o|5|5|4|3|4|5|6|
|!|6|6|5|4|4|5|6|

Örneğin yukarıdaki tablonun 4. satırındaki en küçük değer 3 olduğundan altta kalan satırların kontrolüne gerek kalmadan kelimelerin mesafelerinin ölçülmesi sonlandırılabilir.





