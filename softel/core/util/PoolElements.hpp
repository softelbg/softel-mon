#pragma once

namespace SF {
namespace CORE {
namespace UTIL {


template < typename TElement >
class PoolElements
{
public:
	PoolElements()
		: dtor_(false)
	{

	}

    PoolElements(const PoolElements& frompool)
        : dtor_(false), pool_(frompool.pool_)
    {
        DLOGF;
    }

	virtual ~PoolElements()
	{
		dtor_ = true;
	}

	void Init(int nummsg)
	{
		pool_.reset(new tbb::concurrent_queue<std::shared_ptr <TElement>>());
		for (int i = 0; i < nummsg; i++)
		{
			std::shared_ptr < TElement > element(
				new TElement(),
				boost::bind(&PoolElements::Destroy, this, _1));

			pool_->push(element);
		}
	}

	std::shared_ptr < TElement > Create()
	{
		std::shared_ptr <TElement> element;
		if (! pool_->try_pop(element))
		{
			throw std::runtime_error("Pool: No element");
		}

		return element;
	}

	void Destroy(TElement * element)
	{
		if (!element)
		{
			throw std::runtime_error("Pool: The element is not valid");
		}

		if (dtor_)
		{
			delete element;
		}
		else
		{
			pool_->push(
				std::shared_ptr <TElement>(element,
				boost::bind(&PoolElements::Destroy, this, _1)));
		}
	}

    size_t UnsafeSize() const
    {
        return pool_->unsafe_size();
    }

private:
	bool dtor_;
	std::shared_ptr<tbb::concurrent_queue<std::shared_ptr <TElement>>> pool_;
};


} /* namespace UTIL */
} /* namespace CORE */
} /* namespace SF */

