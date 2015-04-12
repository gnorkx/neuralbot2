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

    nnet_TrainIn_ = new fann_type*[nnet_TrainSteps_];
    nnet_TrainOut_ = new fann_type*[nnet_TrainSteps_];
    for(int i = 0; i<nnet_TrainSteps_; i++)
    {
        nnet_TrainIn_[i] = new fann_type[nnet_nInputs_];
        nnet_TrainOut_[i] = new fann_type[nnet_nOutputs_];
        for(int j= 0; j<nnet_nInputs_; j++)
            nnet_TrainIn_[i][j] = rhs.nnet_TrainIn_[i][j];
        for(int j = 0; j<nnet_nOutputs_; j++)
            nnet_TrainOut_[i][j] = rhs.nnet_TrainOut_[i][j];
    }


}

bot::bot(bot &&rhs)
{
    copy_bot(rhs);

    nnet_ = rhs.nnet_;
    rhs.nnet_ = nullptr;

    sensors_.swap(rhs.sensors_);

    nnet_TrainIn_ = rhs.nnet_TrainIn_;
    nnet_TrainOut_ = rhs.nnet_TrainOut_;
    rhs.nnet_TrainIn_ = nullptr;
    rhs.nnet_TrainOut_ = nullptr;

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

if(nnet_TrainIn_ && nnet_TrainOut_)
    {
        for(int i = 0; i<nnet_TrainSteps_; i++)
        {
            delete[] nnet_TrainIn_[i];
            delete[] nnet_TrainOut_[i];
            nnet_TrainIn_[i] = nullptr;
            nnet_TrainOut_[i] = nullptr;
        }
        delete[] nnet_TrainIn_;
        delete[] nnet_TrainOut_;

        nnet_TrainIn_ = nullptr;
        nnet_TrainOut_ = nullptr;
    }

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


    //move this part into new class
    for(int i = std::min((unsigned)nnet_nInputs_,Lifetime_)-1; i>0; i--)
    {
        for(int j = 0; j<nnet_nInputs_; j++)
            nnet_TrainIn_[i][j] = nnet_TrainIn_[i-1][j];
        for(int j = 0; j<nnet_nOutputs_; j++)
            nnet_TrainOut_[i][j] = nnet_TrainOut_[i-1][j];
    }

    for(int j =0; j<nnet_nInputs_; j++)
        nnet_TrainIn_[0][j] = nnet_input[j];

    for(int j =0; j<nnet_nOutputs_; j++)
        nnet_TrainOut_[0][j] = nnet_out[j];




    object* TouchedFood = touch();
    if(TouchedFood)
    {
        eat((food*)TouchedFood);
        learn();
    }

    renderer::Instance()->Circle(Pos_, size_, c_);
    renderer::Instance()->Line(Pos_,Pos_+DistClosest,c_);
    cout<<Pos_<<endl;
}

object* bot::touch()
{
    //move this into sensor
   if(gWorld.size()>0)
    {
        std::partial_sort(gWorld.begin(),
            gWorld.begin()+1,
            gWorld.end(),
            [=]( object *a,  object *b){ float Diff_a = a->active_?(a->Pos_- Pos_).abs():9999.;
                                 float Diff_b = b->active_?(b->Pos_ - Pos_).abs():9999;
                                return Diff_a<Diff_b;}
                                );


        Coord Diff = gWorld[0]->Pos_ - Pos_;
        if(Diff.abs()<size_+gWorld[0]->size_ && gWorld[0]->active_)
            return gWorld[0];
    }

    return nullptr;
}

void bot::eat(food* food)
{
    if(!food) return;

    Life_ += food->life_;
    food->SetInactive();
}

void bot::learn()
{
    unsigned TrainSteps = std::min((unsigned) nnet_TrainSteps_,Lifetime_);
    if(TrainSteps > 0)
    {
        FANN::training_data tmp_tr;
        tmp_tr.set_train_data(TrainSteps,nnet_nInputs_,nnet_TrainIn_,nnet_nOutputs_,nnet_TrainOut_);
//        if(tr.length_train_data()>0)
//            tr.merge_train_data(tmp_tr);
//        else
        tmp_tr.set_train_data(TrainSteps,nnet_nInputs_,nnet_TrainIn_,nnet_nOutputs_,nnet_TrainOut_);
    }
}


void bot::init()
{

    c_ = {255,255,0};
    size_ = 1;
    sensors_.push_back(new simple_eyes);

    nSensorOut_ = 0;
    for(auto it = sensors_.begin(); it!= sensors_.end(); ++it)
        nSensorOut_ += (*it)->nVal_;

    nnet_= new neural_net;
    const unsigned neurons[] = {nSensorOut_,nnet_nOutputs_};
    nnet_->create_standard_array(2,neurons);
    nnet_->set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
    nnet_->set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);



    nnet_TrainIn_ = new fann_type*[nnet_TrainSteps_];
    nnet_TrainOut_ = new fann_type*[nnet_TrainSteps_];
    for(int i = 0; i<nnet_TrainSteps_; i++)
    {
        nnet_TrainIn_[i] = new fann_type[nnet_nInputs_];
        nnet_TrainOut_[i] = new fann_type[nnet_nOutputs_];
        for(int j= 0; j<nnet_nInputs_; j++)
            nnet_TrainIn_[i][j] = 0;
        for(int j = 0; j<nnet_nOutputs_; j++)
            nnet_TrainOut_[i][j] = 0;
    }

}

