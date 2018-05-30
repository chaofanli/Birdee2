#pragma once
namespace Birdee
{
	template<typename Derived, typename Base, typename Del>
	std::unique_ptr<Derived, Del>
		static_unique_ptr_cast(std::unique_ptr<Base, Del>&& p)
	{
		auto d = static_cast<Derived *>(p.release());
		return std::unique_ptr<Derived, Del>(d, std::move(p.get_deleter()));
	}

	template<typename Derived, typename Base>
	std::unique_ptr<Derived>
		dynamic_unique_ptr_cast(std::unique_ptr<Base>&& p)
	{
		if (Derived *result = dynamic_cast<Derived *>(p.get())) {
			p.release();
			return std::unique_ptr<Derived>(result);
		}
		return std::unique_ptr<Derived>(nullptr);
	}
	template<typename Derived, typename Base>
	bool instance_of(Base* p)
	{
		if (Derived *result = dynamic_cast<Derived *>(base)) {
			return true;
		}
		return false;
	}
}