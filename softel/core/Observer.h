#include <tuple>
#include <utility>
 
#include <boost/signals2.hpp>
 
template<typename Observers> class Observable {
private:
    using ObserverTable = typename Observers::ObserverTable; 
public:
    // Registers an observer.
    template<size_t ObserverId, typename F>
    boost::signals2::connection
    Register(F&& f)
    {
        return std::get<ObserverId>(signals_).connect(std::forward<F>(f));
    } 
protected:
    Observable() = default;
    
    // Notifies observers.
    template<size_t ObserverId, typename... Args>
    typename std::tuple_element<ObserverId, ObserverTable>::type::result_type
    Notify(Args&&... args) 
    {
        return std::get<ObserverId>(signals_)(std::forward<Args>(args)...);
    }
    
private:
    ObserverTable signals_;
};
