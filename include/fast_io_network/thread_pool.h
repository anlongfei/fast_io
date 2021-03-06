#pragma once

namespace fast_io
{

/*
https://www.youtube.com/watch?v=c1gO9aB9nbs&t=3166s
CppCon 2014: Herb Sutter "Lock-Free Programming (or, Juggling Razor Blades), Part I"
*/
template<std::movable acceptor_type,std::size_t size=900,typename server_type,typename Func>
inline void thread_pool_accept(server_type& server,Func&& func)
{
	std::vector<std::thread> pool;
	pool.reserve(size);
	std::array<std::pair<std::mutex,std::condition_variable>,size> cvs;
	std::vector<std::optional<acceptor_type>> slot(size);
	for(std::size_t i{};i!=size;++i)
		pool.emplace_back([func,&cvs,&slot](std::size_t i)
		{
			auto &cvp(cvs[i]);
			for(std::optional<acceptor_type>& opt(slot[i]);;)
			{
				std::unique_lock ul{cvp.first};
				cvp.second.wait(ul,[&opt](){return opt.has_value();});
				auto acc(*std::move(opt));
				opt.reset();
				func(acc);
			}
		},i);
	for(auto i(slot.begin());;)
	{
		std::optional<acceptor_type> opt(std::in_place,server);
		for(auto e(slot.end());*i;)
			if(++i==e)
				i=slot.begin();
		*i=std::move(opt);
		cvs[i-slot.begin()].second.notify_one();
	}
}

}