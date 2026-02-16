#ifndef MPPT_H
#define MPPT_H

#include <QObject>

class TagList;
class TagSocket;

#include <memory>

namespace plugin {

class Mppt : public QObject
{
	Q_OBJECT
  public:
	Mppt(TagList *taglist, const QString &serialNumber);

	void reset();
	int yield() const { return daylyYield_; }

  private slots:
	void onYieldChanged(int value);

  private:
	std::shared_ptr<TagSocket> yieldTagSocket_;

	int daylyYield_ = 0;
};

} // namespace plugin
#endif // MPPT_H
