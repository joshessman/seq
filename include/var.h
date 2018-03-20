#ifndef SEQ_VAR_H
#define SEQ_VAR_H

#include <cstdlib>
#include <map>
#include "types.h"
#include "stage.h"
#include "pipeline.h"

namespace seq {
	class Seq;
	class LoadStore;

	class Var {
	protected:
		Stage *stage;
	public:
		Var();
		Var(Pipeline pipeline);

		virtual types::Type *getType(Stage *caller) const;
		virtual std::shared_ptr<std::map<SeqData, llvm::Value *>> outs(Stage *caller) const;
		virtual Stage *getStage() const;
		virtual bool isAssigned() const;
		virtual Seq *getBase() const;

		virtual Pipeline operator|(Pipeline to);
		virtual Var& operator=(Pipeline to);

		virtual LoadStore& operator[](Var& idx);
	};

	class Latest : public Var {
	private:
		Latest();
	public:
		Latest(Latest const&)=delete;
		void operator=(Latest const&)=delete;

		types::Type *getType(Stage *caller) const override;
		std::shared_ptr<std::map<SeqData, llvm::Value *>> outs(Stage *caller) const override;
		Stage *getStage() const override;
		bool isAssigned() const override;
		Seq *getBase() const override;

		static Latest& get();
	};

	static Latest& _ = Latest::get();
}

#endif /* SEQ_VAR_H */