//
// Created by phoenixflower on 7/19/20.
//

#ifndef LANDESSDEVCORE_SCOPEDRENDERCONTEXTSTATE_HPP
#define LANDESSDEVCORE_SCOPEDRENDERCONTEXTSTATE_HPP
#include "TermBoxRenderingContext.hpp"
namespace LD
{
    namespace TUI
    {
        class ScopedRenderingContextState
        {
        private:
            LD::ElementReference<LD::TermBoxRenderContext> mContext;
            LD::TermBoxConsoleApplicationColorSpectrum CurrentSpectrum;
            LD::TermBoxConsoleApplicationColorModifier CurrentForegroundModifier;
            LD::TermBoxConsoleApplicationColorModifier CurrentBackgroundModifier;
            unsigned short CurrentForegroundColor;
            unsigned short CurrentBackgroundColor;
        public:
             ScopedRenderingContextState(const LD::ElementReference<LD::TermBoxRenderContext> & context) noexcept :mContext(context)
            {
                this->CurrentSpectrum = this->mContext->GetSpectrum();
                this->CurrentForegroundModifier =  this->mContext->GetForegroundModifier();
                this->CurrentBackgroundModifier = this->mContext->GetBackgroundModifier();
                this->CurrentForegroundColor = this->mContext->GetForegroundColor();
                this->CurrentBackgroundColor = this->mContext->GetBackgroundColor();
            }
            ~ScopedRenderingContextState() noexcept
            {
                this->mContext->SetCurrentSpectrum(this->CurrentSpectrum);
                this->mContext->SetForegroundCurrentModifier(this->CurrentForegroundModifier);
                this->mContext->SetBackgroundCurrentModifier(this->CurrentBackgroundModifier);
                this->mContext->SetForegroundColor(this->CurrentForegroundColor);
                this->mContext->SetBackgroundColor(this->CurrentBackgroundColor);
            }
        };
    }
}
#endif //LANDESSDEVCORE_SCOPEDRENDERCONTEXTSTATE_HPP
