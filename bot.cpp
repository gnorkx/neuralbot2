#include "bot.h"
#include "simple_eyes.h"

extern vector<object*> gWorld;

bot::bot(Coord Pos)
{
    Pos_ = Pos;
    Vel_ = {0,0};
    init();
}
bot::bot(const bot &rhs)
{
    copy_bot(rhs);
    copy_nn(rhs);

    sensors_ = rhs.sensors_; //need to clone sensor objects..s.?
}

bot::bot(bot &&rhs)
{
    copy_bot(rhs);

    nnet_ = rhs.nnet_;
    rhs.nnet_ = nullptr;

    sensors_.swap(rhs.sensors_);

}

void bot::copy_bot(const bot &rhs)
{
    Pos_        = rhs.Pos_;
    Direction_  = rhs.Direction_;
    Vel_        = rhs.Vel_;
    c_          = rhs.c_;

    Life_       = rhs.Life_;
    MaxLife_    = rhs.MaxLife_;

    Lifetime_   = rhs.Lifetime_;

}

void bot::copy_nn(const bot &rhs)
{
    nnet_ = new neural_net;
    *nnet_ = *rhs.nnet_;
}

bot::~bot()
{

    //delete sensors:
//    for(auto it = sensors_.begin(); it!= sensors_.end();++it)
//        delete *it;

delete nnet_;

}


void bot::update()
{

//    Vel_ = {rnd0(),rnd0()};
//    Pos_ += Vel_;

    int nVal;
    float *Values = nullptr;
    fann_type nnet_input[nSensorOut_];

    Coord DistClosest;
    for(auto it = sensors_.begin(); it!= sensors_.end();++it)
    {
        (*it)->update(*this, nVal, Values);
        DistClosest = {Values[0],Values[1]};
        for(int i = 0; i<nVal; i++)
        {
            nnet_input[i] = Values[i];
        }
    }

    fann_type *nnet_out = nnet_->run(nnet_input);

    Vel_ = {nnet_out[0], nnet_out[1]};
    Pos_+=Vel_;

    Lifetime_++;

    renderer::Instance()->Circle(Pos_, size_, c_);
    renderer::Instance()->Line(Pos_,Pos_+DistClosest,c_);
    cout<<Pos_<<endl;
}

void bot::init()
{

    c_ = {255,255,0};
    size_ = 1;
    sensors_.push_back(new simple_eyes);

    nSensorOut_ = 0;
    for(auto it = sensors_.begin(); it!= sensors_.end(); ++it)
        nSensorOut_ += sensors_->nVal_;

    nnet_= new neural_net;
    const unsigned neurons[] = {nSensorOut_,2};
    nnet_->create_standard_array(2,neurons);
    nnet_->set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
    nnet_->set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);

}

