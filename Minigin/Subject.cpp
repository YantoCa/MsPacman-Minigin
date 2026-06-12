#include "Subject.h"
#include "Observer.h"

namespace dae {
	void Subject::AddObserver(Observer* observer)
	{
		m_Observers.emplace_back(observer);
	}

	void Subject::RemoveObserver(Observer* observer)
	{
		m_Observers.erase(std::find(begin(m_Observers), end(m_Observers), observer));
	}

	void Subject::Notify(const GameObject& object, const std::string& event)
	{
		for (auto const& observer : m_Observers)
		{
			observer->OnNotify(object, event);
		}
	}
}