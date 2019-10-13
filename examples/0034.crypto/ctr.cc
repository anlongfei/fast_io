
#include"../../include/fast_io.h"

#include <array>
#include <string>
#include<iterator>
#include<vector>
#include <deque>



int main()
try
{
    std::array<uint8_t, 8> nonce = {'1','2','3','4','1','2','3','4'};
    std::array<uint8_t, 16> key = {'8','3','3','4',';','2','3','4','a','2','c','4',']','0','3','4'};
    {
        fast_io::obuf ob("ctr.out");
        fast_io::crypto::basic_octr<decltype(ob), fast_io::crypto::speck::speck_enc_128_128> enc_stream(key, nonce, ob);
		//std::vector<std::size_t> vec = { 1,2,3,4,5,6 };
		enc_stream.put('a');
        print(enc_stream, "bc");
		print(enc_stream, "defghijklmnop12345678123456");
		print(enc_stream, "78ponmlkjihgfedcba");
		print_flush(enc_stream, "xx");
		//write(enc_stream, vec);
    }
    
    {
        fast_io::ibuf ib("ctr.out");
		fast_io::nobuf_reader<fast_io::crypto::basic_ictr<decltype(ib), fast_io::crypto::speck::speck_enc_128_128>> dec_stream(key, nonce, ib);
		transmit(fast_io::out, dec_stream);
		//std::vector<std::size_t> vec;
		//read(dec_stream, vec);
		//for (auto const& x : vec) {
		//	println(fast_io::out, x);
		//}
    }
    
}
catch(std::exception const & e)
{
	println(fast_io::err,e);
	return 1;
}


