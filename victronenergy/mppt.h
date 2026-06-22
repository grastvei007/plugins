#ifndef MPPT_H
#define MPPT_H

#include <QObject>
#include <tagsystem/global/victron.h>

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
	double yield() const { return daylyYield_; }
	double dayilyBulk() const { return yield_.bulk_;}
	double daylyAbsorption() const { return yield_.absorption_;}
	double daylyFloat() const { return yield_.float_;}

  private slots:
	void onYieldChanged(double value);
	void onStateOfOperationChanged(int state);

  private:
	std::shared_ptr<TagSocket> yieldTagSocket_;
	std::shared_ptr<TagSocket> stateOfOperationTagSocket_;

	struct Yield{
		double bulk_ = 0.0;
		double absorption_ = 0.0;
		double float_ = 0.0;
	};

	double daylyYield_ = 0;
	Yield yield_;

	global::victron::CS stateOfOperation_ = global::victron::CS::Off;
};

} // namespace plugin
#endif // MPPT_H
