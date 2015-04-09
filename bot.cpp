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

    nn_train_in = new fann_type*[g_max_nn_train];
    nn_train_out = new fann_type*[g_max_nn_train];
    for(int i = 0; i<g_max_nn_train; i++)
    {
        nn_train_in[i] = new fann_type[g_max_nn_in];
        nn_train_out[i] = new fann_type[g_max_nn_out];
        for(int j= 0; j<g_max_nn_in; j++)
            nn_train_in[i][j] = rhs.nn_train_in[i][j];
        for(int j = 0; j<g_max_nn_out; j++)
            nn_train_out[i][j] = rhs.nn_train_out[i][j];
    }


}

bot::bot(bot &&rhs)
{
    copy_bot(rhs);

    nnet_ = rhs.nnet_;
    rhs.nnet_ = nullptr;

    sensors_.swap(rhs.sensors_);

    nn_train_in = rhs.nn_train_in;
    nn_train_out = rhs.nn_train_out;
    rhs.nn_train_in = nullptr;
    rhs.nn_train_out = nullptr;

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

if(nn_train_in && nn_train_out)
    {
        for(int i = 0; i<g_max_nn_train; i++)
        {
            delete[] nn_train_in[i];
            delete[] nn_train_out[i];
            nn_train_in[i] = nullptr;
            nn_train_out[i] = nullptr;
        }
        delete[] nn_train_in;
        delete[] nn_train_out;

        nn_train_in = nullptr;
        nn_train_out = nullptr;
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

    object* TouchedFood = touch();
    if(TouchedFood)
    {
        eat(TouchedFood);
        learn();
    }

    renderer::Instance()->Circle(Pos_, size_, c_);
    renderer::Instance()->Line(Pos_,Pos_+DistClosest,c_);
    cout<<Pos_<<endl;
}

object* bot::touch()
{
   if(gWorld.size()>0)
    {
        std::partial_sort(gWorld.begin(),
            gWorld.begin()+1,
            gWorld.end(),
            [=]( object *a,  object *b){ float Diff_a = a->active_?(a->Pos_-bot_.Pos_).abs():9999.;
                                 float Diff_b = b->active?(b->Pos_ - bot_.Pos_).abs():9999;
                                return Diff_a<Diff_b;}
                                );


        Coord Diff = gWorld[0]->Pos_ - bot_.Pos_;
        if(Diff.abs()<bot.size_+gWorld[0]->size_ && gWorld[0]->active_)
            return gWorld[0];
    }

    return nullptr;
}

void bot::eat(object* food)
{
    if(!food) return;
    Life_ += food->life_;
    food->SetInactive();
}

void bot::learn()
{
    unsigned TrainSteps = std::min((unsigned) g_max_nn_train,Lifetime);
    if(TrainSteps > 0)
    {
        FANN::training_data tmp_tr;
        tmp_tr.set_train_data(TrainSteps,g_max_nn_in,nn_train_in,g_max_nn_out,nn_train_out);
        if(tr.length_train_data()>0)
            tr.merge_train_data(tmp_tr);
        else
            tr.set_train_data(TrainSteps,g_max_nn_in,nn_train_in,g_max_nn_out,nn_train_out);
    }
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

    nn_train_in = new fann_type*[g_max_nn_train];
        nn_train_out = new fann_type*[g_max_nn_train];
        for(int i = 0; i<g_max_nn_train; i++)
        {
            nn_train_in[i] = new fann_type[g_max_nn_in];
            nn_train_out[i] = new fann_type[g_max_nn_out];
            for(int j= 0; j<g_max_nn_in; j++)
                nn_train_in[i][j] = 0;
            for(int j = 0; j<g_max_nn_out; j++)
                nn_train_out[i][j] = 0;
        }

}

