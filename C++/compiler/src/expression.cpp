#include <unordered_set>
#include <sstream>
#include "expression.hpp"
#include "term_factory.hpp"

namespace ntt {

    Expression::Expression(Tokenizer& tokenizer) {

        static std::unordered_set<std::string> ops {
            "+" ,  "-" ,  "*" ,  "/" ,  "&" ,  "|" ,  "<" ,  ">" ,  "="
        };

        first_term_ = TermFactory::parse(tokenizer);

        while(true) {
            if(!tokenizer.has_token())
                break;
            
            const auto& token = tokenizer.peek();
            if(!ops.count(token.value()))
                break;

            auto op = tokenizer.get();
            trailing_terms_.emplace_back(op, TermFactory::parse(tokenizer));
        }
    }

    std::string Expression::to_xml(size_t level) const {
        std::ostringstream oss;

        oss << JackFragment::get_line("<expression>", level);
        oss << first_term_->to_xml(level + 1);
        for(const auto& [op, term] : trailing_terms_) {
            oss << op.to_xml(level + 1, JackFragment::TAB_WIDTH_) << std::endl;
            oss << term->to_xml(level + 1);
        }
        oss << JackFragment::get_line("</expression>", level);

        return oss.str();
    }

}
